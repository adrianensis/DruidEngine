#include "Core/Engine.hpp"
#include "Scene/Scene.hpp"
#include "Scene/GameObject.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Scripting/Script.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Animation.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "GameController.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Maths/Vector4.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/ScenesManager.hpp"

using namespace DE;

int main() {

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	// OBJECT 1

	GameObject* controller = DE_NEW<GameObject>();
	controller->init();
	controller->setTag("controller");

	// script

	Script* script = DE_NEW<GameController>();
	controller->addComponent<Script>(script);
	Script* scriptX = controller->getFirstComponent<Script>();

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	DE_FREE(Engine::getInstance());
	Memory::free();

	return 0;
}
