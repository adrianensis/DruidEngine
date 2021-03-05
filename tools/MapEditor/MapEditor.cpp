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

#include "Config/EngineConfig.hpp"
#include "Config/ConfigMap.hpp"
#include "Events/EventsManager.hpp"
#include "MapElement/MapElement.hpp"

namespace DE {

MapEditor::MapEditor() : Script() {

}

MapEditor::~MapEditor() = default;

void MapEditor::createPlayer() {
	Vector2 size(100 * 1.5f, 100 * 1.5f);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	mPlayer = DE_NEW<GameObject>();
	mPlayer->init();

	mPlayer->getTransform()->setLocalPosition(Vector3(-300, -100, 0));
	mPlayer->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = DE_NEW<Renderer>();
	mPlayer->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

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

	RigidBody* rigidBody = DE_NEW<RigidBody>();
	mPlayer->addComponent<RigidBody>(rigidBody);

	Collider* collider = DE_NEW<Collider>();
	mPlayer->addComponent<Collider>(collider);
	collider->setSize(size.x / 1.5f, size.y/3.0f);
	collider->setPositionOffset(Vector3(0,-size.y/2,0));

	getGameObject()->getScene()->addGameObject(mPlayer);
}

void MapEditor::destroyPlayer(){
	getGameObject()->getScene()->removeGameObject(mPlayer);
	mPlayer = nullptr;
}

void MapEditor::init() {
	DE_INSTANCEABLE_BY_CLASSNAME(MapElement);
	DE_INSTANCEABLE_BY_CLASSNAME(MapElement_Tile);
	DE_INSTANCEABLE_BY_CLASSNAME(MapElement_ActionPoint);

	mTransform = getGameObject()->getTransform();

	mConfigMap = DE_NEW<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/editor.conf");

	mCameraControl = true;
	mLayer = 0;
	mZoom = 1;
}

void MapEditor::firstStep() {

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getFirstComponent<Camera>();
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mGrid.init(this, mConfigMap->getF32("grid.size"), mConfigMap->getF32("grid.tile.size"));

	//mGrid.loadMapIntoGrid(getGameObject()->getScene()->getNewGameObjects());
	//mGrid.loadMapIntoGrid(getGameObject()->getScene()->getGameObjects());

	mIsMapLoaded = false;

	mMapEditorUI.init(this);

	//createPlayer();
}

void MapEditor::step() {

	if(mIsMapLoaded){

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

	} else if(getGameObject()->getScene()->isLoadFinished()){
		mGrid.loadMapIntoGrid(getGameObject()->getScene()->getGameObjects());
		mIsMapLoaded = true;
	}
}

void MapEditor::addColliderToTile(GameObject *tile) {
	if(tile){
		List<RigidBody*>* rigidBodies = tile->getComponents<RigidBody>();

		if(!rigidBodies || (rigidBodies && rigidBodies->isEmpty())){
			RigidBody* rigidBody = DE_NEW<RigidBody>();
			tile->addComponent<RigidBody>(rigidBody);

			Collider* collider = DE_NEW<Collider>();
			tile->addComponent<Collider>(collider);
			collider->setSize(tile->getTransform()->getScale().x, tile->getTransform()->getScale().y);

			getGameObject()->getScene()->updateComponents(tile);
		}
	}
}

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

void MapEditor::switchCameraControl() {
mCameraControl = !mCameraControl;
}

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

void MapEditor::terminate() {


	DE_FREE(mConfigMap);
}

}
