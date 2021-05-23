#include "Core/Engine.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"

#include <iostream>

int main(){

	Engine* engine = Engine::getInstance();

	engine->init();

	GameObject* controller = new GameObject;
	controller->init();

	//Script* script = Memory::allocate<Playground>();
	//controller->addComponent<Script>(script);

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Engine::deleteInstance();

	return 0;
}
