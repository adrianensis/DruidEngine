#include "Engine.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Script.hpp"
#include "Transform.hpp"
#include "Animation.hpp"
#include "Renderer.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "Vector4.hpp"
#include "ScenesManager.hpp"

#include "Playground.hpp"

using namespace DE;

int main() {

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

	Script* script = Memory::allocate<Playground>();
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
