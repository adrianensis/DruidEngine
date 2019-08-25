#include "Druid.h"
#include "CustomScript.h"

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

	cameraGameObject->getTransform()->setLocalPosition(Vector3(0,0,-100));

	// script

	//Script* script = Memory::allocate<Script>();
	//cameraGameObject->addComponent<Script>(script);

	Camera* cameraComponent = Memory::allocate<Camera>();
	cameraGameObject->addComponent<Camera>(cameraComponent);
	cameraComponent->setOrtho(-1024, 1024, -768, 768, 1000, -1000);
	//cameraComponent->setPerspective(100, -100, 800/600, 90);

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
	texture->init("resources/terrain.png");

	Material* material = Memory::allocate<Material>();
	material->init();
	material->setTexture(texture);

	// OBJECT 1

	GameObject* gameObject = Memory::allocate<GameObject>();
  gameObject->init();

	gameObject->getTransform()->setLocalPosition(Vector3(0,0,0));
	gameObject->getTransform()->setScale(Vector3(300,300,1));

	// script

	Script* script = Memory::allocate<CustomScript>();
	gameObject->addComponent<Script>(script);

	Renderer* renderer = Memory::allocate<Renderer>();
	gameObject->addComponent<Renderer>(renderer);

	renderer->setMesh(mesh);
	renderer->setMaterial(material);

	f32 tileSize = (1.0f/16.0f);
	renderer->setRegion(0*tileSize,2*tileSize,(1.0f),tileSize);

	renderer->addAnimation("testAnimation", Animation::create(16, true, false, Vector2(0,0), 1.0f/16.0f, 1.0f, 1));
	renderer->setAnimation("testAnimation");

	scene->setCameraGameObject(cameraGameObject);

	scene->addGameObject(gameObject);

	scene->addGameObject(cameraGameObject);

	// OBJECT 2

	Texture* texture2 = Memory::allocate<Texture>();
	texture2->init("resources/snorlax.bmp");

	Material* material2 = Memory::allocate<Material>();
	material2->init();
	material2->setTexture(texture2);

	for (size_t i = 0; i < 200; i++) {

		GameObject* gameObject2 = Memory::allocate<GameObject>();
	  gameObject2->init();

		gameObject2->getTransform()->setLocalPosition(Vector3((rand() * 100) % 1000,(rand() * 100) % 1000,20));
		gameObject2->getTransform()->setScale(Vector3(200,200,1));
		gameObject2->getTransform()->setRotation(Vector3(0,0,0));

		// script

		// Script* script2 = Memory::allocate<Script>();
		// gameObject2->addComponent<Script>(script2);

		Renderer* renderer2 = Memory::allocate<Renderer>();
		gameObject2->addComponent<Renderer>(renderer2);

		renderer2->setMesh(mesh);
		renderer2->setMaterial(material2);

		//scene->addGameObject(gameObject2);

	}

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
