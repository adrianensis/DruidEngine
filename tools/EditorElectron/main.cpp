#include "Core/Engine.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"
#include "Scripting/Script.hpp"

#include "Server/EditorServer.hpp"

#include "Config/ConfigObject.hpp"

#include <iostream>

int main()
{
	Engine *engine = Engine::getInstance();

	engine->init();

	GameObject *controller = NEW(GameObject);
	controller->init();

	Script *script = NEW(EditorServer);
	controller->addComponent<Script>(script);

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Engine::deleteInstance();

	return 0;
}
