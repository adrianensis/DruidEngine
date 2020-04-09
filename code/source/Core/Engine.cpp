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
#include "MaterialManager.h"

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

	RenderContext::init();
	Settings::getInstance()->init();
	MaterialManager::getInstance()->init();

  mScenes = Memory::allocate<List<Scene*>>();
  mScenes->init();
}

// ---------------------------------------------------------------------------

void Engine::initSubsystems(){

	if(mScriptEngine) mScriptEngine->terminate();
	if(mRenderEngine) mRenderEngine->terminate();
	if(mPhysicsEngine) mPhysicsEngine->terminate();
	if(UI::getInstance()) UI::getInstance()->terminate();

	mRenderEngine = RenderEngine::getInstance();
	mScriptEngine = ScriptEngine::getInstance();
	mPhysicsEngine = PhysicsEngine::getInstance();

	f32 sceneSize = mScenes->get(mCurrentSceneIndex)->getSize();

	if(sceneSize == 0) {
		sceneSize = Settings::getInstance()->getF32("scene.defaultSize");
	}

	mRenderEngine->init(sceneSize);
	mScriptEngine->init();
	mPhysicsEngine->init(sceneSize);
	UI::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::addScene(Scene* newScene){
	mScenes->pushBack(newScene);
}

// ---------------------------------------------------------------------------

void Engine::setScene(u32 i){
	mCurrentSceneIndex = i;

	if(Settings::getInstance()->getU32("scenes.length") > 0){
		std::string sceneName = Settings::getInstance()->getString("scenes["+std::to_string(mCurrentSceneIndex)+"]");

		Scene* scene = mScenes->get(mCurrentSceneIndex);
		scene->loadScene(sceneName);
	}
}

// ---------------------------------------------------------------------------

void Engine::loadScene(Scene* scene){
	// TRACE();

	Camera* camera = scene->getCameraGameObject()->getComponents<Camera>()->get(0);

	mRenderEngine->setCamera(camera);

	const List<GameObject*>* newGameObjects = scene->getNewGameObjects();
	u32 maxToSpawn = Settings::getInstance()->getF32("scene.maxNewObjectsToSpawn");

	// VAR(f32, newGameObjects->getLength());

	FOR_LIST (itGameObjects, newGameObjects){
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
	Time::init();

	f32 accumulator = 0.0f;

	f32 FPS = 60.0f; // TODO : GLFW is capped to 60 fps.
	f32 inverseFPS = 1.0f/FPS;

	initSubsystems();

	while(! RenderContext::isClosed()) {

		Time::tick();

		if(mScenes->get(mCurrentSceneIndex)->thereAreNewGameObjects()){
			loadScene(mScenes->get(mCurrentSceneIndex));
		}

		Input::pollEvents();

		UI::getInstance()->step();

		mScriptEngine->step();

		mPhysicsEngine->step(Time::getDeltaTimeSeconds());

		mRenderEngine->step();

		// std::cout << (1.0f/Time::getDeltaTimeSeconds()) << std::endl;
		// VAL(f32, 1.0f/Time::getDeltaTimeSeconds());
	}

	mScriptEngine->terminate();
	mRenderEngine->terminate();
	mPhysicsEngine->terminate();
	UI::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

void Engine::terminate() {

	Memory::free<RenderEngine>(mRenderEngine);
	Memory::free<MaterialManager>(MaterialManager::getInstance());

	Memory::free<ScriptEngine>(mScriptEngine);

	FOR_LIST (it, mScenes){
		Memory::free<Scene>(it.get());
	}

	Memory::free<List<Scene*>>(mScenes);
	Memory::free<Settings>(Settings::getInstance());

	Memory::free();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
