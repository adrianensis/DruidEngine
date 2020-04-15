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
#include "ScenesManager.h"

#include <string>
#include <iostream>

#include <thread>

namespace DE {

// ---------------------------------------------------------------------------

Engine::Engine() : DE_Class(), Singleton(){
	mFPS = 60;
	mRenderEngine = nullptr;
	mPhysicsEngine = nullptr;
	mScriptEngine = nullptr;
}

Engine::~Engine() = default;

// ---------------------------------------------------------------------------

void Engine::init(){

	TRACE()

	RenderContext::init();
	Settings::getInstance()->init();
	MaterialManager::getInstance()->init();

	ScenesManager::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::initSubsystems(){

	terminateSubSystems();

	mRenderEngine = RenderEngine::getInstance();
	mScriptEngine = ScriptEngine::getInstance();
	mPhysicsEngine = PhysicsEngine::getInstance();

	f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();

	if(sceneSize == 0){
		sceneSize = Settings::getInstance()->getF32("scene.defaultSize");
	}

	mRenderEngine->init(sceneSize);
	mScriptEngine->init();
	mPhysicsEngine->init(sceneSize);
	UI::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::terminateSubSystems(){

	// ScenesManager::getInstance()->getCurrentScene()->unloadScene();

	if(mScriptEngine) mScriptEngine->terminate();
	if(mRenderEngine) mRenderEngine->terminate();
	if(mPhysicsEngine) mPhysicsEngine->terminate();
	if(UI::getInstance()) UI::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

void Engine::run(){
	Time::init();

	f32 accumulator = 0.0f;

	f32 FPS = 60.0f; // TODO : GLFW is capped to 60 fps.
	f32 inverseFPS = 1.0f/FPS;

	// initSubsystems();

	while(! RenderContext::isClosed()){

		Time::tick();

		if(ScenesManager::getInstance()->sceneHasChanged()){
			initSubsystems();
		}

		ScenesManager::getInstance()->step();

		Input::pollEvents();

		UI::getInstance()->step();

		mScriptEngine->step();

		f32 dtHalf = Time::getDeltaTimeSeconds()/2.0f;

		mPhysicsEngine->step(dtHalf);
		mPhysicsEngine->step(dtHalf);
		// mPhysicsEngine->step(Time::getDeltaTimeSeconds()/3.0f);
		mPhysicsEngine->updateContacts();

		mRenderEngine->step();

		std::cout << (1.0f/Time::getDeltaTimeSeconds()) << std::endl;
		// VAL(f32, 1.0f/Time::getDeltaTimeSeconds());
	}
}

// ---------------------------------------------------------------------------

void Engine::terminate(){

	terminateSubSystems();

	RenderContext::terminate();

	Memory::free<RenderEngine>(mRenderEngine);
	Memory::free<MaterialManager>(MaterialManager::getInstance());

	Memory::free<ScriptEngine>(mScriptEngine);

	Memory::free<Settings>(Settings::getInstance());
	Memory::free<ScenesManager>(ScenesManager::getInstance());
}

// ---------------------------------------------------------------------------

} /* namespace DE */
