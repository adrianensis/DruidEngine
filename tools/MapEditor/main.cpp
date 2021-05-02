#include "Core/Engine.hpp"
#include "Scene/Scene.hpp"
#include "Scene/GameObject.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Scripting/Script.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Maths/Vector4.hpp"
#include "Scene/ScenesManager.hpp"

#include "../MapEditor/MapEditor.hpp"

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

	Memory::free(Engine::getInstance());
	Memory::free();

	return 0;
}
