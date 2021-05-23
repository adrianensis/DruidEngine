#include "Core/Engine.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"

#include "Config/ConfigObject.hpp"

#include <iostream>

int main(){

	Engine* engine = Engine::getInstance();

	engine->init();

	GameObject* controller = new GameObject;
	controller->init();

	// JSON TEST
	// ----------------------------------------------------
	// ----------------------------------------------------
	ConfigObject* configMap = new ConfigObject;
	configMap->init();

	configMap->getJson()["objects"].push_back(JSON::object());
	configMap->getJson()["objects"].push_back(JSON::object());
	configMap->getJson()["objects"].push_back(JSON::object());

	//configMap->getJson()["objects"] = JsonObjects;

	configMap->writeConfigFile("config/test.json");

	// ----------------------------------------------------
	// ----------------------------------------------------

	//Script* script = Memory::allocate<Playground>();
	//controller->addComponent<Script>(script);

	ScenesManager::getInstance()->setGameObjectController(controller);

	//engine->run();

	engine->terminate();

	Engine::deleteInstance();

	return 0;
}
