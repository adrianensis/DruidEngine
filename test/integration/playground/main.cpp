#include "Core/Engine.hpp"
#include "Core/Time/TimerManager.hpp"
#include "Config/ConfigMap.hpp"

#include <iostream>

int main(){

	Engine* engine = Engine::getInstance();

	engine->init();

	engine->run();

	engine->terminate();

	std::cout << std::boolalpha;

	ObjectBase* ob1 = new ObjectBase;
	ObjectBase* ob2 = new ObjectBase;

	/*std::cout << engine->getClassName() << std::endl;
	std::cout << ob1->getClassName() << std::endl;
	std::cout << ob2->getClassName() << std::endl;
	std::cout << std::endl;
	std::cout << engine->getClassId() << std::endl;
	std::cout << ob1->getClassId() << std::endl;
	std::cout << ob2->getClassId() << std::endl;
	std::cout << std::endl;
	std::cout << engine->getObjectId() << std::endl;
	std::cout << ob1->getObjectId() << std::endl;
	std::cout << ob2->getObjectId() << std::endl;*/

	/*CLASS(Test, ObjectBase){

		PRI(A, GET_SET, f32)
		PRI(B, GET_SET, f32)

	public:

		COPY(){
			DO_COPY(A)
			DO_COPY(B)
		}

	};

	Test a;
	Test b;

	a.setA(1);
	b.setB(2);

	a = b;

	std::cout << a.getA() << std::endl;
	std::cout << a.getB() << std::endl;
	std::cout << std::endl;
	std::cout << b.getA() << std::endl;
	std::cout << b.getB() << std::endl;*/

	delete engine;

	return 0;
}
