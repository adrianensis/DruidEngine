#include "Druid.h"

using namespace DE;

int main() {

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	Scene* scene = Memory::allocate<Scene>();

  scene->init();

	// camera

	GameObject* cameraGameObject = Memory::allocate<GameObject>();
	cameraGameObject->init();

	cameraGameObject->getTransform()->setLocalPosition(Vector3(0,0,0));

	// script

	Script* script = Memory::allocate<Script>();
	cameraGameObject->addComponent(script);

	Camera* cameraComponent = Memory::allocate<Camera>();
	cameraComponent->setOrtho(-800, 800, -600, 600, 1, -1);
	//cameraComponent->setPerspective(10, -10, 800/600, 45);
	cameraGameObject->addComponent(cameraComponent);

	// render

	Mesh* mesh = Memory::allocate<Mesh>();
	mesh->init(4);

	mesh->open()->
		addVertex(Vector3(-0.5f, 0.5f, 0.0f))-> // top left
		addVertex(Vector3(-0.5f, -0.5f, 0.0f))-> // bottom left
		addVertex(Vector3(0.5f, -0.5f, 0.0f))-> // bottom right
		addVertex(Vector3(0.5f, 0.5f, 0.0f))-> // top right

		addTexCoord(0.0f, 1.0f)-> // top left
		addTexCoord(0.0f, 0.0f)-> // bottom left
		addTexCoord(1.0f, 0.0f)-> // bottom right
		addTexCoord(1.0f, 1.0f)-> // top right

		addColor(Vector4(0, 1, 1, 1))-> // top left
		addColor(Vector4(1, 0, 0, 1))-> // bottom left
		addColor(Vector4(1, 1, 0, 1))-> // bottom right
		addColor(Vector4(0, 0, 1, 1))-> // top right

		addFace(0,1,3)->
		addFace(1,2,3)->
		close();

	// Material

	Texture* texture = Memory::allocate<Texture>();
	texture->init("resources/char.png");

	Material* material = Memory::allocate<Material>();
	material->setTexture(texture);

	// OBJECT 1

	GameObject* gameObject = Memory::allocate<GameObject>();
  gameObject->init();

	gameObject->getTransform()->setLocalPosition(Vector3(0,0,0));
	gameObject->getTransform()->setScale(Vector3(300,300,1));

	// script

	// Script* script = Memory::allocate<Script>();
	// gameObject->addComponent(script);

	Renderer* renderer = Memory::allocate<Renderer>();

	renderer->setMesh(mesh);
	renderer->setMaterial(material);

	gameObject->addComponent(renderer);

	// OBJECT 2

	GameObject* gameObject2 = Memory::allocate<GameObject>();
  gameObject2->init();

	gameObject2->getTransform()->setLocalPosition(Vector3(-300,-300,0));
	gameObject2->getTransform()->setScale(Vector3(100,100,1));

	// script

	// Script* script2 = Memory::allocate<Script>();
	// gameObject2->addComponent(script2);

	Renderer* renderer2 = Memory::allocate<Renderer>();

	Texture* texture2 = Memory::allocate<Texture>();
	texture2->init("resources/snorlax.bmp");

	Material* material2 = Memory::allocate<Material>();
	material2->setTexture(texture2);

	renderer2->setMesh(mesh);
	renderer2->setMaterial(material2);

	gameObject2->addComponent(renderer2);

	scene->setCameraGameObject(cameraGameObject);

	scene->addGameObject(gameObject);
	scene->addGameObject(gameObject2);
	scene->addGameObject(cameraGameObject);

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
