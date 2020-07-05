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
#include "GameController.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "Vector4.hpp"
#include "RenderContext.hpp"
#include "ScenesManager.hpp"

using namespace DE;

int main() {

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	// OBJECT 1

	GameObject* controller = Memory::allocate<GameObject>();
	controller->init();
	controller->setTag("controller");

	// script

	Script* script = Memory::allocate<GameController>();
	controller->addComponent<Script>(script);
	Script* scriptX = controller->getComponents<Script>()->get(0);

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Memory::free<Engine>(Engine::getInstance());
	Memory::free();

	return 0;
}
