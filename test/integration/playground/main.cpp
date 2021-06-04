#include "Core/Engine.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"
#include "Scripting/Script.hpp"

#include "Playground.hpp"

#include "Config/ConfigObject.hpp"

#include <iostream>

/*#include <ft2build.h>
#include FT_FREETYPE_H */

int main(){

	Engine* engine = Engine::getInstance();

	engine->init();

	GameObject* controller = NEW(GameObject);
	controller->init();

	Script* script = NEW(Playground);
	controller->addComponent<Script>(script);

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Engine::deleteInstance();

	return 0;
}
