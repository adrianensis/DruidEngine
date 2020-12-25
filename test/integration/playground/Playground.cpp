#include "Playground.hpp"

#include <TimeUtils.hpp>
#include "Log.hpp"

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Input.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "Array.hpp"

#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "MaterialManager.hpp"
#include "Animation.hpp"

#include "Collider.hpp"
#include "RigidBody.hpp"

#include "Scene.hpp"
#include "RenderEngine.hpp"
#include "RenderContext.hpp"

#include "File.hpp"
#include "UI.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"

#include "Settings.hpp"
#include "ProjectileScript.hpp"

namespace DE {

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

Playground::Playground() : Script() {

}

// ---------------------------------------------------------------------------

Playground::~Playground() = default;

// ---------------------------------------------------------------------------

void Playground::createPlayer() {
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

	renderer->setLayer(1);

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
	collider->setSize(size.x / 1.5f, size.y);
	collider->setCollisionLayer(1);

	getGameObject()->getScene()->addGameObject(mPlayer);
}

// ---------------------------------------------------------------------------

GameObject* Playground::createProjectile(f32 x, f32 y) {
	Vector2 size(40,40);

	GameObject* tile = Memory::allocate<GameObject>();
	tile->init();

	tile->getTransform()->setLocalPosition(mPlayer->getTransform()->getWorldPosition());
	tile->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	tile->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mMaterial);

	Vector2 atlasSize = Vector2(8, 16);

	renderer->setRegion(4/atlasSize.x, 0, 1/atlasSize.x, 1/atlasSize.y);

	renderer->setLayer(2);
	//renderer->setColor(Vector4(1,0.0f,0.0f,1));

	//tile->setIsStatic(true);
	//tile->setShouldPersist(true);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	tile->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	tile->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->setShape(ColliderShape::SPHERE);

	Vector3 direction = Vector3(mPlayer->getTransform()->getWorldPosition()).sub(Vector3(x, y, 0));
	direction.nor();

	rigidBody->setLinear(-direction * 50);

	ProjectileScript* script = Memory::allocate<ProjectileScript>();
	tile->addComponent<Script>(script);

	getGameObject()->getScene()->addGameObject(tile);

	return tile;
}


// ---------------------------------------------------------------------------

void Playground::init() {
	mTransform = getGameObject()->getTransform();
	mPlayer = nullptr;
	mMaterial = nullptr;

	mCamera = nullptr;
	mCameraTransform = nullptr;


	mCameraControl = true;
}

// ---------------------------------------------------------------------------

void Playground::firstStep() {

	//RenderEngine::getInstance()->setDebugColliders(true);

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mMaterial = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	u32 i = 0;
	FOR_LIST(it, getGameObject()->getScene()->getNewGameObjects()){
		it.get()->setTag("obj" + std::to_string(i));
		++i;
	}

	createPlayer();
	mCameraControl = false;
	//createTile(300,300,30, 30);


}

// ---------------------------------------------------------------------------

void Playground::step() {

	if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)) {

		Vector2 mouse(Input::getInstance()->getMousePosition());
		Vector3 world = mCamera->screenToWorld(mouse);

		Vector2 projectileOrigin = Vector2(getGameObject()->getTransform()->getWorldPosition());

		createProjectile(world.x, world.y);

	}

	processMovement();

}


void Playground::processMovement() {

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

void Playground::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
