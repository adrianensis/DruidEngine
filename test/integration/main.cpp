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
#include "GameController.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector4.h"
#include "RenderContext.h"
#include "ScenesManager.h"

using namespace DE;

int main(){

	Memory::init();

	Engine* engine = Engine::getInstance();

	engine->init();

	// OBJECT 1

	GameObject* controller = Memory::allocate<GameObject>();
  controller->init();

	// script

	Script* script = Memory::allocate<GameController>();
	controller->addComponent<Script>(script);

	ScenesManager::getInstance()->setGameObjectController(controller);

	engine->run();

	engine->terminate();

	Memory::free<Engine>(Engine::getInstance());
	Memory::free();

	return 0;
}
