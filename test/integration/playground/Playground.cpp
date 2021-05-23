#include "Playground.hpp"

#include "Core/Time/TimeUtils.hpp"
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
//#include "Input/Input.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"

#include "ProjectileScript.hpp"

void Playground::createPlayer() {
	Vector2 size(400, 400 );

	// Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage.png");

	mPlayer = new GameObject;
	mPlayer->init();

	mPlayer->getTransform()->setLocalPosition(Vector3(0, 0, 0));
	mPlayer->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = new Renderer;
	mPlayer->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	// renderer->setMaterial(material);
	renderer->setMaterial(mMaterial);

	renderer->setLayer(1);

	Vector2 atlasSize = Vector2(8, 16);

	/*renderer->addAnimation("idle",
			Animation::create(6, true, false, Vector2(0, 14.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->addAnimation("run",
			Animation::create(6, true, false, Vector2(0, 15.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->setAnimation("idle");*/

	//renderer->setIsLineMode(true);

	getGameObject()->getScene()->addGameObject(mPlayer);
}

void Playground::init() {
	mTransform = getGameObject()->getTransform();
	mPlayer = nullptr;
	mMaterial = nullptr;

	mCamera = nullptr;
	mCameraTransform = nullptr;

	mCameraControl = true;
}

void Playground::firstStep() {

	//RenderEngine::getInstance()->setDebugColliders(true);

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getFirstComponent<Camera>();
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mMaterial = MaterialManager::getInstance()->loadMaterial("resources/snorlax.png");

	u32 i = 0;
	FOR_LIST(it, *getGameObject()->getScene()->getNewGameObjects()){
		(*it)->setTag("obj" + std::to_string(i));
		++i;
	}

	createPlayer();
	mCameraControl = false;
	//createTile(300,300,30, 30);

}

void Playground::step() {

	processMovement();

}

void Playground::processMovement() {

	f32 movement = (mCameraControl ? 2000.0f * Time::getInstance()->getDeltaTimeSeconds() : 500.0f);

	if (!mCameraControl) {
		mCameraTransform->setParent(mPlayer->getTransform());
		mCameraTransform->setLocalPosition(Vector3(0, 0, 0));
	}

	Renderer* renderer = mCameraControl ? nullptr : mPlayer ? mPlayer->getFirstComponent<Renderer>() : nullptr;

	/*if (Input::getInstance()->isKeyPressed(GLFW_KEY_UP)) {
		if(mCameraControl){
			mCameraTransform->translate(Vector3(0, movement, 0));
		} else {
			mPlayer->getFirstComponent<RigidBody>()->setLinear(Vector3(0,movement,0));

			if (renderer) {
				renderer->setAnimation("run");
			}
		}

	} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_DOWN)) {
		if(mCameraControl){
			mCameraTransform->translate(Vector3(0, -movement, 0));
		} else {
			mPlayer->getFirstComponent<RigidBody>()->setLinear(Vector3(0,-movement,0));

			if (renderer) {
				renderer->setAnimation("run");
			}
		}
	} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT)) {
		if(mCameraControl){
			mCameraTransform->translate(Vector3(-movement, 0, 0));
		} else {
			mPlayer->getFirstComponent<RigidBody>()->setLinear(Vector3(-movement,0,0));

			if (renderer) {
				renderer->setAnimation("run");
				renderer->setIsInvertAxis(true);
			}
		}
	} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_RIGHT)) {
		if(mCameraControl){
			mCameraTransform->translate(Vector3(movement, 0, 0));
		} else {
			mPlayer->getFirstComponent<RigidBody>()->setLinear(Vector3(movement,0,0));

			if (renderer) {
				renderer->setAnimation("run");
				renderer->setIsInvertAxis(false);
			}
		}
	} else {
		if(!mCameraControl){
			mPlayer->getFirstComponent<RigidBody>()->setLinear(Vector3(0,0,0));

			if (renderer) {
				renderer->setAnimation("idle");
			}
		}
	}*/
}

void Playground::terminate() {

}