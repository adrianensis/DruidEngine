#include "Engine.hpp"
#include "Memory.hpp"
#include "Time2.hpp"
#include "List.hpp"
#include "Singleton.hpp"
#include "RenderContext.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "RenderEngine.hpp"
#include "PhysicsEngine.hpp"
#include "ScriptEngine.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Script.hpp"
#include "Camera.hpp"
#include "Log.hpp"
#include "Input.hpp"
#include "RigidBody.hpp"
#include "UI.hpp"
#include "Settings.hpp"
#include "MaterialManager.hpp"
#include "ScenesManager.hpp"

#include <string>
#include <iostream>

#include <thread>

namespace DE {

// ---------------------------------------------------------------------------

Engine::Engine() : DE_Class(), Singleton() {
	mFPS = 60;
	mRenderEngine = nullptr;
	mPhysicsEngine = nullptr;
	mScriptEngine = nullptr;
}

Engine::~Engine() = default;

// ---------------------------------------------------------------------------

void Engine::init() {

	TRACE()

	RenderContext::init();
	Settings::getInstance()->init();
	MaterialManager::getInstance()->init();

	ScenesManager::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::initSubsystems() {

	terminateSubSystems();

	mRenderEngine = RenderEngine::getInstance();
	mScriptEngine = ScriptEngine::getInstance();
	mPhysicsEngine = PhysicsEngine::getInstance();

	f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();

	if (sceneSize == 0) {
		sceneSize = Settings::getInstance()->getF32("scene.defaultSize");
	}

	mRenderEngine->init(sceneSize);
	mScriptEngine->init();
	mPhysicsEngine->init(sceneSize);
	UI::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::terminateSubSystems() {

	// ScenesManager::getInstance()->getCurrentScene()->unloadScene();

	if (mScriptEngine)
		mScriptEngine->terminate();
	if (mRenderEngine)
		mRenderEngine->terminate();
	if (mPhysicsEngine)
		mPhysicsEngine->terminate();
	if (UI::getInstance())
		UI::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

void Engine::run() {
	Time::init();

	f32 accumulator = 0.0f;

	f32 FPS = 60.0f; // TODO : GLFW is capped to 60 fps.
	f32 inverseFPS = 1.0f / FPS;

	//initSubsystems();

	double lasttime = glfwGetTime();

	while (!RenderContext::isClosed()) {

		Time::tick();

		if (ScenesManager::getInstance()->sceneHasChanged()) {
			initSubsystems();
		}

		ScenesManager::getInstance()->step();

		Input::pollEvents();

		UI::getInstance()->step();

		mScriptEngine->step();

		mPhysicsEngine->step(Time::getDeltaTimeSeconds());
		//mPhysicsEngine->updateContacts();

		mRenderEngine->step();

		// std::cout << (1.0f/Time::getDeltaTimeSeconds()) << std::endl;
		// VAL(f32, 1.0f/Time::getDeltaTimeSeconds());

		while (glfwGetTime() < lasttime + 1.0/FPS) {
		        // TODO: Put the thread to sleep, yield, or simply do nothing
		    }
		    lasttime += 1.0/FPS;
	}
}

// ---------------------------------------------------------------------------

void Engine::terminate() {

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
