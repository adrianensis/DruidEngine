#include "Core/Engine.hpp"

#include <iostream>

int main() {

	Engine* engine = Engine::getInstance();

	engine->init();

	engine->run();

	engine->terminate();

	std::cout << std::boolalpha;

	ObjectBase* ob1 = new ObjectBase;
	ObjectBase* ob2 = new ObjectBase;

	std::cout << engine->getClassName() << std::endl;
	std::cout << ob1->getClassName() << std::endl;
	std::cout << ob2->getClassName() << std::endl;
	std::cout << std::endl;
	std::cout << engine->getClassId() << std::endl;
	std::cout << ob1->getClassId() << std::endl;
	std::cout << ob2->getClassId() << std::endl;
	std::cout << std::endl;
	std::cout << engine->getObjectId() << std::endl;
	std::cout << ob1->getObjectId() << std::endl;
	std::cout << ob2->getObjectId() << std::endl;

	delete engine;

	return 0;
}
