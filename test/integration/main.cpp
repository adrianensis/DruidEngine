#include "Engine.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Script.h"
#include "Transform.h"
#include "Animation.h"
#include "Renderer.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "CustomScript.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector4.h"

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

	//Script* script = Memory::allocate<Script>();
	//cameraGameObject->addComponent<Script>(script);

	Camera* cameraComponent = Memory::allocate<Camera>();
	cameraGameObject->addComponent<Camera>(cameraComponent);
	cameraComponent->setOrtho(-1024, 1024, -768, 768, 1000, -1000);
	//cameraComponent->setPerspective(100, -100, 800/600, 90);

	// render

	// Material

	Texture* texture = Memory::allocate<Texture>();
	texture->init("resources/mage.bmp");

	Material* material = Memory::allocate<Material>();
	material->init();
	material->setShader(Shader::getDefaultShader());
	material->setTexture(texture);

	// OBJECT 1

	GameObject* gameObject = Memory::allocate<GameObject>();
  gameObject->init();

	Vector2 size(300,300);

	gameObject->getTransform()->setLocalPosition(Vector3(-200,200,0));
	gameObject->getTransform()->setScale(Vector3(size.x,size.y,1));

	Renderer* renderer = Memory::allocate<Renderer>();
	gameObject->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	// f32 tileSize = (1.0f/16.0f);
	// renderer->setRegion(0*tileSize,2*tileSize,(1.0f),tileSize);

	renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
	renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
	renderer->setAnimation("idle");

	renderer->setColor(Vector4(0,0,0,1));

	renderer->setLayer(1);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	gameObject->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	gameObject->addComponent<Collider>(collider);
	collider->setSize(size.x,size.y);

	// script

	Script* script = Memory::allocate<CustomScript>();
	gameObject->addComponent<Script>(script);

	scene->setCameraGameObject(cameraGameObject);

	scene->addGameObject(gameObject);

	scene->addGameObject(cameraGameObject);

	// OBJECT 2

	Texture* texture2 = Memory::allocate<Texture>();
	texture2->init("resources/snorlax.bmp");

	Material* material2 = Memory::allocate<Material>();
	material2->init();
	material2->setTexture(texture2);
	material2->setShader(Shader::getDefaultShader());


	for (size_t i = 0; i < 200; i++) {

		// GameObject* gameObject2 = Memory::allocate<GameObject>();
	  // gameObject2->init();
		//
		// gameObject2->getTransform()->setLocalPosition(Vector3((rand() * 100) % 1000,(rand() * 100) % 1000,20));
		// gameObject2->getTransform()->setScale(Vector3(200,200,1));
		// gameObject2->getTransform()->setRotation(Vector3(0,0,0));
		//
		// Renderer* renderer2 = Memory::allocate<Renderer>();
		// gameObject2->addComponent<Renderer>(renderer2);
		//
		// renderer2->setMesh(Mesh::getRectangle());
		// renderer2->setMaterial(material2);

		// script

		// Script* script2 = Memory::allocate<Script>();
		// gameObject2->addComponent<Script>(script2);

		//scene->addGameObject(gameObject2);

	}

	engine->addScene(scene);
	engine->setScene(0);

	engine->run();

	engine->terminate();

	return 0;
}
