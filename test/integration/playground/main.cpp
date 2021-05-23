#include "Core/Engine.hpp"

#include <iostream>

int main(){

	Engine* engine = Engine::getInstance();

	engine->init();

	engine->run();

	engine->terminate();

	Engine::deleteInstance();

	return 0;
}
