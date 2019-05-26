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

	// script

	Script* script = Memory::allocate<Script>();
	gameObject->addComponent<Script>(script);

	// render

	Mesh* mesh = Memory::allocate<Mesh>();
	mesh->init(4);

	mesh->open()->
		addVertex(Vector3(-0.5f, 0.5f, 0.0f))->
		addVertex(Vector3(-0.5f, -0.5f, 0.0f))->
		addVertex(Vector3(0.5f, -0.5f, 0.0f))->
		addVertex(Vector3(0.5f, 0.5f, 0.0f))->
		addFace(0,1,3)->
		addFace(1,2,3)->
		close();

	Renderer* renderer = Memory::allocate<Renderer>();

	renderer->setMesh(mesh);

	gameObject->addComponent<Renderer>(renderer);

	scene->addGameObject(gameObject);

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
