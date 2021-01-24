#include "MapEditor.hpp"

#include "Core/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation.hpp"

#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"

#include "Config/EngineSettings.hpp"
#include "Config/ConfigMap.hpp"
#include "Events/EventsManager.hpp"

namespace DE {

// ---------------------------------------------------------------------------

MapEditor::MapEditor() : Script() {

}

// ---------------------------------------------------------------------------

MapEditor::~MapEditor() = default;

// ---------------------------------------------------------------------------

void MapEditor::createPlayer() {
	Vector2 size(100 * 1.5f, 100 * 1.5f);

	// Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage.png");

	mPlayer = Memory::allocate<GameObject>();
	mPlayer->init();

	mPlayer->getTransform()->setLocalPosition(Vector3(-300, -100, 0));
	mPlayer->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mPlayer->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	// renderer->setMaterial(material);
	renderer->setMaterial(mMaterial);

	renderer->setLayer(mLayer);

	Vector2 atlasSize = Vector2(8, 16);

	renderer->addAnimation("idle",
			Animation::create(6, true, false, Vector2(0, 14.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->addAnimation("run",
			Animation::create(6, true, false, Vector2(0, 15.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->setAnimation("idle");

	//renderer->setIsLineMode(true);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	mPlayer->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	mPlayer->addComponent<Collider>(collider);
	collider->setSize(size.x / 1.5f, size.y/3.0f);
	collider->setPositionOffset(Vector3(0,-size.y/2,0));

	getGameObject()->getScene()->addGameObject(mPlayer);
}

void MapEditor::destroyPlayer(){
	getGameObject()->getScene()->removeGameObject(mPlayer);
	mPlayer = nullptr;
}

// ---------------------------------------------------------------------------

void MapEditor::init() {
	mTransform = getGameObject()->getTransform();

	mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/editor.conf");

	mCameraControl = true;
	mLayer = 0;
	mZoom = 1;
}

// ---------------------------------------------------------------------------

void MapEditor::firstStep() {

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getFirstComponent<Camera>();
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mMaterial = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	mGrid.init(getGameObject()->getScene(), mConfigMap->getF32("grid.size"), mConfigMap->getF32("grid.tile.size"));

	//mGrid.loadMapIntoGrid(getGameObject()->getScene()->getNewGameObjects());
	mGrid.loadMapIntoGrid(getGameObject()->getScene()->getGameObjects());

	mMapEditorUI.init(this);

	//createPlayer();
}

// ---------------------------------------------------------------------------

void MapEditor::step() {

	//click(clampedPosition);
	if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)
	|| Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

		if(RenderEngine::getInstance()->getLayersData()->get(mLayer)->mVisible){
			FOR_RANGE(i, 0, mMapEditorUI.mBrush.mBrushGridSize.y){
				FOR_RANGE(j, 0, mMapEditorUI.mBrush.mBrushGridSize.x){
					f32 offset = mGrid.getGridTileSize();

					Vector2 mouse(Input::getInstance()->getMousePosition());
					Vector3 world = mCamera->screenToWorld(mouse);

					u32 sceneSize = getGameObject()->getScene()->getSize() / 2.0f;
					if(sceneSize >= std::abs(world.x) && sceneSize >= std::abs(world.y)){
						Vector3 clampedPosition(std::roundf(world.x / mGrid.getGridTileSize()) * mGrid.getGridTileSize(), std::roundf(world.y / mGrid.getGridTileSize()) * mGrid.getGridTileSize(), 0);

						//ECHO("CLICK")
						// TODO : how many times is this called?
						mGrid.click(clampedPosition + Vector3(offset*j, -offset*i, 0), mMapEditorUI.mBrush.getTile(i, j), mMapEditorUI.mBrush.mDrawTileSize, mLayer);
					}
				}
			}
		}
	}

	cameraZoom();
	processMovement();

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_Z)) {
		if (mLayer < 10)
			mLayer++;
	}

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_X)) {
		if (mLayer > 0)
			mLayer--;
	}

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_ESCAPE)) {
		mMapEditorUI.resetBrush();
	}

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_TAB)) {
		mMapEditorUI.toggleAtlas();
	}

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_S) && Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL /*| GLFW_MOD_SHIFT*/)) {
		getGameObject()->getScene()->saveScene(getGameObject()->getScene()->getPath());
	}

	mMapEditorUI.update();
}

// ---------------------------------------------------------------------------

void MapEditor::addColliderToTile(GameObject *tile) {
	if(tile){
		List<RigidBody*>* rigidBodies = tile->getComponents<RigidBody>();

		if(!rigidBodies || (rigidBodies && rigidBodies->isEmpty())){
			RigidBody* rigidBody = Memory::allocate<RigidBody>();
			tile->addComponent<RigidBody>(rigidBody);

			Collider* collider = Memory::allocate<Collider>();
			tile->addComponent<Collider>(collider);
			collider->setSize(tile->getTransform()->getScale().x, tile->getTransform()->getScale().y);

			getGameObject()->getScene()->updateComponents(tile);
		}
	}
}

// ---------------------------------------------------------------------------

void MapEditor::cameraZoom() {
	f32 scroll = Input::getInstance()->getScroll();
	mZoom += std::fabs(scroll) * 0.05f * Time::getInstance()->getDeltaTimeSeconds();

	f32 finalZoom = mZoom;
	if(Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		finalZoom = mZoom * 2.0f;
	}

	if (scroll == 1) {
		mCamera->setZoom(1.0f / finalZoom);
	} else if (scroll == -1) {
		mCamera->setZoom(finalZoom);
	}
}

// ---------------------------------------------------------------------------

void MapEditor::switchCameraControl() {
mCameraControl = !mCameraControl;
}

// ---------------------------------------------------------------------------

void MapEditor::processMovement() {

f32 movement = (mCameraControl ? 2000.0f * Time::getInstance()->getDeltaTimeSeconds() : 500.0f);

if (!mCameraControl) {
mCameraTransform->setParent(mPlayer->getTransform());
mCameraTransform->setLocalPosition(Vector3(0, 0, 0));
}

Renderer* renderer = mCameraControl ? nullptr : mPlayer ? mPlayer->getComponents<Renderer>()->get(0) : nullptr;

if (Input::getInstance()->isKeyPressed(GLFW_KEY_UP)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(0, movement, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,movement,0));

		if (renderer) {
			renderer->setAnimation("run");
		}
	}

} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_DOWN)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(0, -movement, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,-movement,0));

		if (renderer) {
			renderer->setAnimation("run");
		}
	}
} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(-movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(-movement,0,0));

		if (renderer) {
			renderer->setAnimation("run");
			renderer->setIsInvertAxis(true);
		}
	}
} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_RIGHT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(movement,0,0));

		if (renderer) {
			renderer->setAnimation("run");
			renderer->setIsInvertAxis(false);
		}
	}
} else {
	if(!mCameraControl){
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,0,0));

		if (renderer) {
			renderer->setAnimation("idle");
		}
	}
}
}

// ---------------------------------------------------------------------------

void MapEditor::terminate() {



	Memory::free<ConfigMap>(mConfigMap);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
