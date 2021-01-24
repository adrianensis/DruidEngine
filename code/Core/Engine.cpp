#include "Core/TimeUtils.hpp"
#include "Core/Engine.hpp"
#include "Memory/Memory.hpp"
#include "Containers/List.hpp"
#include "Core/Singleton.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Scripting/Script.hpp"
#include "Graphics/Camera.hpp"
#include "Log/Log.hpp"
#include "Input/Input.hpp"
#include "Physics/RigidBody.hpp"
#include "UI/UI.hpp"
#include "Config/EngineSettings.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Scene/ScenesManager.hpp"
#include "Events/EventsManager.hpp"
#include "Core/TimerManager.hpp"
#include "Profiler/Profiler.hpp"

#include <string>
#include <iostream>

#include <thread>
#include <chrono>

#include "Config/InstanceableByClassName.hpp"

using namespace std::chrono_literals;

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

	DE_TRACE()

	loadMainConfig();

	RenderContext::init();

	TimerManager::getInstance()->init();
	Profiler::getInstance()->init();
	EventsManager::getInstance()->init();

	EngineSettings::getInstance()->init();
	MaterialManager::getInstance()->init();

	ScenesManager::getInstance()->init();
}

// ---------------------------------------------------------------------------

void Engine::initSubsystems() {

	f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();

	// TODO : set default scene size if scene size is 0

	RenderEngine::getInstance()->init(sceneSize);
	ScriptEngine::getInstance()->init();
	PhysicsEngine::getInstance()->init(sceneSize);
	UI::getInstance()->init();

}

// ---------------------------------------------------------------------------

void Engine::terminateSubSystems() {

	// ScenesManager::getInstance()->getCurrentScene()->unloadScene();

	ScriptEngine::getInstance()->terminate();
	RenderEngine::getInstance()->terminate();
	PhysicsEngine::getInstance()->terminate();
	UI::getInstance()->terminate();
	//EventsManager::getInstance()->terminate();
	TimerManager::getInstance()->terminate();
	//Profiler::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

void Engine::run() {
	Time::getInstance()->init();

	f32 FPS = 60.0f; // TODO : Move to settings.
	f32 inverseFPS = 1.0f / FPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (!RenderContext::isClosed()) {

		Time::getInstance()->startFrame();

		if (ScenesManager::getInstance()->getSceneHasChanged()) {
			terminateSubSystems();
			ScenesManager::getInstance()->loadCurrentScene();
			initSubsystems();
		}

		ScenesManager::getInstance()->step();

		Input::getInstance()->pollEvents();

		TimerManager::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());

		ScriptEngine::getInstance()->step();

		PhysicsEngine::getInstance()->step(inverseFPS);

		RenderEngine::getInstance()->step();

		//std::cout << " " << Time::getInstance()->getElapsedTime() << std::endl;
		f32 dt = (Time::getInstance()->getElapsedTime());

		diff = inverseFPSMillis - dt;

		if(inverseFPSMillis > dt){
			//std::cout << dt/1000.0f;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff/**1000.0f*/));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}

		Time::getInstance()->endFrame();

		Profiler::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());
		//std::cout << " " << 1.0f/Time::getInstance()->getDeltaTimeSeconds() << std::endl;


	}
}

// ---------------------------------------------------------------------------

void Engine::terminate() {

	terminateSubSystems();

	RenderContext::terminate();

	Memory::free<RenderEngine>(RenderEngine::getInstance());
	Memory::free<MaterialManager>(MaterialManager::getInstance());
	Memory::free<ScriptEngine>(ScriptEngine::getInstance());
	Memory::free<PhysicsEngine>(PhysicsEngine::getInstance());
	Memory::free<UI>(UI::getInstance());
	Memory::free<EngineSettings>(EngineSettings::getInstance());
	Memory::free<ScenesManager>(ScenesManager::getInstance());
	Memory::free<Time>(Time::getInstance());

	if (EventsManager::getInstance()){
		Memory::free<EventsManager>(EventsManager::getInstance());
	}

	if (Profiler::getInstance()){
		Memory::free<Profiler>(Profiler::getInstance());
	}

	if (TimerManager::getInstance()){
		Memory::free<TimerManager>(TimerManager::getInstance());
	}

	Memory::free<Input>(Input::getInstance());
}

// ---------------------------------------------------------------------------

void Engine::loadMainConfig() {
	registerAllInstaceableByClassName();
}

} /* namespace DE */
