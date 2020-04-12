#include "Engine.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Script.h"
#include "Transform.h"
#include "Animation.h"
#include "Renderer.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "CustomScript.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector4.h"
#include "RenderContext.h"
#include "ScenesManager.h"

using namespace DE;

int main(){

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	// Scene* scene = Memory::allocate<Scene>();
	//
  // scene->init();
	//
	// // camera
	//
	// GameObject* cameraGameObject = Memory::allocate<GameObject>();
	// cameraGameObject->init();
	//
	// cameraGameObject->getTransform()->setLocalPosition(Vector3(0,0,0));
	//
	// Camera* cameraComponent = Memory::allocate<Camera>();
	// cameraGameObject->addComponent<Camera>(cameraComponent);
	//
	// /*
	// if (aspect >= 1.0)
	// 	ortho(-50.0 * aspect, 50.0 * aspect, -50.0, 50.0, 1.0, -1.0);
	// else
	// 	ortho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, 1.0, -1.0);
	// */
	// f32 aspect = RenderContext::getAspectRatio();
	// cameraComponent->setOrtho(-720*aspect, 720*aspect, -720, 720, 1, -1);
	// // cameraComponent->setOrtho(-720, 720, -720, 720, 1000, -1000);

	// OBJECT 1

	GameObject* controller = Memory::allocate<GameObject>();
  controller->init();

	// script

	Script* script = Memory::allocate<CustomScript>();
	controller->addComponent<Script>(script);

	// scene->setCameraGameObject(cameraGameObject);
	//
	// scene->addGameObject(gameObject);
	//
	// scene->addGameObject(cameraGameObject);

	// ScenesManager::getInstance()->addScene(scene);
	// ScenesManager::getInstance()->setScene(0);
	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Memory::free<Engine>(Engine::getInstance());
	Memory::free();

	return 0;
}
