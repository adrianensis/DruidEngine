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
#include "RigidBody.h"
#include "Collider.h"
#include "Vector4.h"
#include "ScenesManager.h"

#include "MapEditor.h"

using namespace DE;

int main(){

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	// Scene* scene = Memory::allocate<Scene>();
	//
  // scene->init();

	// camera

	// GameObject* cameraGameObject = Memory::allocate<GameObject>();
	// cameraGameObject->init();
	//
	// cameraGameObject->getTransform()->setLocalPosition(Vector3(0,0,0));

	// script

	//Script* script = Memory::allocate<Script>();
	//cameraGameObject->addComponent<Script>(script);

	// Camera* cameraComponent = Memory::allocate<Camera>();
	// cameraGameObject->addComponent<Camera>(cameraComponent);
	// cameraComponent->setOrtho(-720, 720, -720, 720, 1, -1);
	//cameraComponent->setPerspective(100, -100, 800/600, 90);

	GameObject* controller = Memory::allocate<GameObject>();
  controller->init();

	// script

	Script* script = Memory::allocate<MapEditor>();
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
