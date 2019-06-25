#include "Druid.h"

using namespace DE;

int main() {

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	Scene* scene = Memory::allocate<Scene>();

  scene->init();

	// render

	Mesh* mesh = Memory::allocate<Mesh>();
	mesh->init(4);

	mesh->open()->
		addVertex(Vector3(-0.5f, 0.5f, 0.0f))-> // top left
		addVertex(Vector3(-0.5f, -0.5f, 0.0f))-> // bottom left
		addVertex(Vector3(0.5f, -0.5f, 0.0f))-> // bottom right
		addVertex(Vector3(0.5f, 0.5f, 0.0f))-> // top right

		addTexCoord(0.0f, 1.0f)-> // top left
		addTexCoord(0.0f, 1.0f)-> // bottom left
		addTexCoord(0.0f, 0.0f)-> // bottom right
		addTexCoord(0.0f, 0.0f)-> // top right

		addColor(Vector4(0, 1, 1, 1))-> // top left
		addColor(Vector4(1, 0, 0, 1))-> // bottom left
		addColor(Vector4(1, 1, 0, 1))-> // bottom right
		addColor(Vector4(0, 0, 1, 1))-> // top right

		addFace(0,1,3)->
		addFace(1,2,3)->
		close();

	// OBJECT 1

	GameObject* gameObject = Memory::allocate<GameObject>();
  gameObject->init();

	gameObject->getTransform()->setLocalPosition(Vector3(0,0,0));

	// script

	Script* script = Memory::allocate<Script>();
	gameObject->addComponent<Script>(script);

	Renderer* renderer = Memory::allocate<Renderer>();

	renderer->setMesh(mesh);

	gameObject->addComponent<Renderer>(renderer);

	// OBJECT 2

	GameObject* gameObject2 = Memory::allocate<GameObject>();
  gameObject2->init();

	gameObject2->getTransform()->setLocalPosition(Vector3(0.5,0.5,0));
	gameObject2->getTransform()->setScale(Vector3(0.5,0.5,1));

	// script

	Script* script2 = Memory::allocate<Script>();
	gameObject2->addComponent<Script>(script2);

	Renderer* renderer2 = Memory::allocate<Renderer>();

	renderer2->setMesh(mesh);

	gameObject2->addComponent<Renderer>(renderer2);

	scene->addGameObject(gameObject);
	scene->addGameObject(gameObject2);

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
