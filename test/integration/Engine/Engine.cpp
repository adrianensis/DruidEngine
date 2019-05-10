#include "Druid.h"

using namespace DE;

int main() {

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	Scene* scene = Memory::allocate<Scene>();
  GameObject* gameObject = Memory::allocate<GameObject>();

  scene->init();

  gameObject->init();

	std::string name = gameObject->getClassName();
	gameObject->getTransform()->setLocalPosition(Vector3(0,0,0));

	Script* script = Memory::allocate<Script>();
	gameObject->addComponent<Script>(script);

	scene->addGameObject(gameObject);

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
