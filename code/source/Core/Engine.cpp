#include "Engine.h"
#include "Memory.h"
#include "Time.h"
#include "List.h"
#include "Singleton.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "Scene.h"
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "ScriptEngine.h"
#include "GameObject.h"
#include "Transform.h"
#include "Script.h"
#include "Camera.h"
#include "Log.h"
#include "Input.h"
#include "RigidBody.h"
#include "UI.h"
#include "Settings.h"

#include <string>
#include <iostream>

#include <thread>

namespace DE {

// ---------------------------------------------------------------------------

Engine::Engine() : DE_Class(), Singleton(){
	mFPS = 60;
	mScenes = nullptr;
	mRenderEngine = nullptr;
	mPhysicsEngine = nullptr;
	mScriptEngine = nullptr;
	mCurrentSceneIndex = 0;
}

Engine::~Engine() = default;

// ---------------------------------------------------------------------------

void Engine::init(){

	TRACE()

	Settings::getInstance()->init();

  mRenderEngine = RenderEngine::getInstance(); //Memory::allocate<RenderEngine>();
  mScriptEngine = ScriptEngine::getInstance(); //Memory::allocate<ScriptEngine>();
  mPhysicsEngine = PhysicsEngine::getInstance(); //Memory::allocate<PhysicsEngine>();
  mScenes = Memory::allocate<List<Scene*>>();

  mRenderEngine->init();
  mScriptEngine->init();
  mPhysicsEngine->init();
  mScenes->init();

	UI::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::addScene(Scene* newScene){
	mScenes->pushBack(newScene);
}

// ---------------------------------------------------------------------------

void Engine::setScene(u32 i){
	mCurrentSceneIndex = i;
}

// ---------------------------------------------------------------------------

void Engine::loadScene(Scene* scene){

	Camera* camera = scene->getCameraGameObject()->getComponents<Camera>()->get(0);

	mRenderEngine->setCamera(camera);

	List<GameObject*>* gameObjects = scene->getNewGameObjects();

	FOR_LIST (itGameObjects, gameObjects){
		GameObject* gameObject = itGameObjects.get();

		List<Script*>* scriptList = gameObject->getComponents<Script>();
		List<Renderer*>* rendererList = gameObject->getComponents<Renderer>();
		List<RigidBody*>* rigidBodyList = gameObject->getComponents<RigidBody>();

		Script* script = scriptList ? scriptList->get(0) : nullptr;
		Renderer* renderer = rendererList ? rendererList->get(0) : nullptr;
		RigidBody* rigidBbody = rigidBodyList ? rigidBodyList->get(0) : nullptr;

		if(script){
			mScriptEngine->addScript(script);
		}

		if(rendererList){
			FOR_LIST (it, rendererList){
				mRenderEngine->addRenderer(it.get());
			}
		}

		if(rigidBbody){
			mPhysicsEngine->addRigidBody(rigidBbody);
		}
	}

	scene->flushNewGameObjects();
}

// ---------------------------------------------------------------------------

void Engine::run(){

	Time::init(); TRACE()

	f32 accumulator = 0.0f;

	f32 FPS = 60.0f; // TODO : GLFW is capped to 60 fps.
	f32 inverseFPS = 1.0f/FPS;

	u32 itCounter = 0;

	while(! RenderContext::isClosed()) {

		Time::tick();

		if(mScenes->get(mCurrentSceneIndex)->thereAreNewGameObjects()){
			loadScene(mScenes->get(mCurrentSceneIndex));
			mRenderEngine->bind();
		}

		/*if(itCounter % 2 == 0) */UI::getInstance()->step();

		/*if(itCounter % 2 == 0) */mScriptEngine->step();

		// while(accumulator >= inverseFPS){
		// 	mScriptEngine->step();
		// 	accumulator -= inverseFPS;
    // }

		mPhysicsEngine->step(Time::getDeltaTimeSeconds());

		mRenderEngine->step();

		// accumulator += Time::getDeltaTimeMillis();

		//VAL(f32, 1.0f/Time::getDeltaTimeSeconds());

		Input::pollEvents();

		Memory::flush();
	}

	mScriptEngine->terminate();
	mRenderEngine->terminate();
	UI::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

void Engine::terminate() {

	Memory::free<RenderEngine>(mRenderEngine);
	Memory::free<ScriptEngine>(mScriptEngine);

	FOR_LIST (it, mScenes){
		Memory::free<Scene>(it.get());
	}

	Memory::free<List<Scene*>>(mScenes);

	Memory::free();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
