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
#include "Config/EngineConfig.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Scene/ScenesManager.hpp"
#include "Events/EventsManager.hpp"
#include "Core/TimerManager.hpp"
#include "Profiler/Profiler.hpp"

#include <string>
#include <iostream>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace DE {

Engine::Engine() : ObjectBase(), Singleton() {
	mFPS = 60; // TODO : Settings?
	mRenderEngine = nullptr;
	mPhysicsEngine = nullptr;
	mScriptEngine = nullptr;
}

Engine::~Engine() = default;

void Engine::init() {

	TRACE()

	RenderContext::init();

	TimerManager::getInstance()->init();
	Profiler::getInstance()->init();
	EventsManager::getInstance()->init();

	EngineConfig::getInstance()->init();
	MaterialManager::getInstance()->init();

	ScenesManager::getInstance()->init();
}

void Engine::initSubsystems() {

	f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();

	RenderEngine::getInstance()->init(sceneSize);
	ScriptEngine::getInstance()->init();
	PhysicsEngine::getInstance()->init(sceneSize);
	UI::getInstance()->init();

}

void Engine::terminateSubSystems() {
	ScriptEngine::getInstance()->terminate();
	RenderEngine::getInstance()->terminate();
	PhysicsEngine::getInstance()->terminate();
	UI::getInstance()->terminate();
	TimerManager::getInstance()->terminate();
}

void Engine::run() {
	Time::getInstance()->init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (!RenderContext::isClosed()) {

		Time::getInstance()->startFrame();

		if (ScenesManager::getInstance()->getSceneHasChanged()) {
			terminateSubSystems();
			ScenesManager::getInstance()->loadCurrentScene();
			initSubsystems();
		}

		Input::getInstance()->pollEvents();
		
		ScenesManager::getInstance()->step();
		TimerManager::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());
		ScriptEngine::getInstance()->step();
		PhysicsEngine::getInstance()->step(inverseFPS);
		RenderEngine::getInstance()->step();

		f32 dtMillis = (Time::getInstance()->getElapsedTimeMillis());

		if(inverseFPSMillis > dtMillis){
			diff = inverseFPSMillis - dtMillis;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}

		Time::getInstance()->endFrame();

		Profiler::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());

	}
}

void Engine::terminate() {

	terminateSubSystems();

	RenderContext::terminate();

	Memory::free(RenderEngine::getInstance());
	Memory::free(MaterialManager::getInstance());
	Memory::free(ScriptEngine::getInstance());
	Memory::free(PhysicsEngine::getInstance());
	Memory::free(UI::getInstance());
	Memory::free(EngineConfig::getInstance());
	Memory::free(ScenesManager::getInstance());
	Memory::free(Time::getInstance());

	if (EventsManager::existsInstance()){
		Memory::free(EventsManager::getInstance());
	}

	if (Profiler::existsInstance()){
		Memory::free(Profiler::getInstance());
	}

	if (TimerManager::existsInstance()){
		Memory::free(TimerManager::getInstance());
	}

	Memory::free(Input::getInstance());
}

}
