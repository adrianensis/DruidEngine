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

#include <string>

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

  mRenderEngine = RenderEngine::getInstance(); //Memory::allocate<RenderEngine>();
  mScriptEngine = ScriptEngine::getInstance(); //Memory::allocate<ScriptEngine>();
  mPhysicsEngine = PhysicsEngine::getInstance(); //Memory::allocate<PhysicsEngine>();
  mScenes = Memory::allocate<List<Scene*>>();

  mRenderEngine->init();
  mScriptEngine->init();
  mPhysicsEngine->init();
  mScenes->init();
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

	mRenderEngine->setCamera((Camera*)scene->getCameraGameObject()->getComponents<Camera>()->get(0));

	List<GameObject*>* gameObjects = scene->getNewGameObjects();

	FOR_LIST (itGameObjects, gameObjects){
		GameObject* gameObject = itGameObjects.get();

		List<Script*>* ScriptList = gameObject->getComponents<Script>();
		List<Renderer*>* RendererList = gameObject->getComponents<Renderer>();
		List<RigidBody*>* RigidBodyList = gameObject->getComponents<RigidBody>();

		Script* script = ScriptList ? ScriptList->get(0) : nullptr;
		Renderer* renderer = RendererList ? RendererList->get(0) : nullptr;
		RigidBody* rigidBbody = RigidBodyList ? RigidBodyList->get(0) : nullptr;

		if(script){
			mScriptEngine->addScript(script);
		}

		if(renderer){
			mRenderEngine->addRenderer(renderer);
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

	while(! RenderContext::isClosed()) {

		if(mScenes->get(mCurrentSceneIndex)->thereAreNewGameObjects()){
			loadScene(mScenes->get(mCurrentSceneIndex));
			mRenderEngine->bind();
		}

		Time::tick();

		while(accumulator >= inverseFPS){
			mScriptEngine->step();
			accumulator -= inverseFPS;
    }

		mPhysicsEngine->step(Time::getDeltaTimeSeconds());

		mRenderEngine->update();
		mRenderEngine->step();

		accumulator += Time::getDeltaTimeMillis();

		//VAL(f32, 1.0f/Time::getDeltaTimeSeconds());

		Input::pollEvents();

		Memory::flush();
	}

	mScriptEngine->terminate();
	mRenderEngine->terminate();
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
