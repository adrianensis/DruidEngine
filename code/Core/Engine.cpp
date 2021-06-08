#include "Core/Time/TimeUtils.hpp"
#include "Core/Time/TimerManager.hpp"
#include "Core/Memory.hpp"
#include "Core/Engine.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/Singleton.hpp"
#include "Input/Input.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Events/EventsManager.hpp"
#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"

#include <string>
#include <iostream>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

Engine::~Engine(){

};

void Engine::init(){
	mFPS = 60;

	Memory::init();

	EngineConfig::getInstance()->init();

	RenderContext::init();

	Input::getInstance()->init();

	TimerManager::getInstance()->init();
	EventsManager::getInstance()->init();

	MaterialManager::getInstance()->init();
	ScenesManager::getInstance()->init();
}

void Engine::initSubsystems(){
	f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();
	RenderEngine::getInstance()->init(sceneSize);
	ScriptEngine::getInstance()->init();
}

void Engine::terminateSubSystems(){

	ScriptEngine::getInstance()->terminate();
	RenderEngine::getInstance()->terminate();

	TimerManager::getInstance()->terminate();
}

void Engine::run(){
	Time::getInstance()->init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (!RenderContext::isClosed()){

		Time::getInstance()->startFrame();

		if (ScenesManager::getInstance()->getSceneHasChanged()) {
			terminateSubSystems();
			ScenesManager::getInstance()->loadCurrentScene();
			initSubsystems();
		}

		Input::getInstance()->pollEvents();

		ScenesManager::getInstance()->step();
		TimerManager::getInstance()->step();
		ScriptEngine::getInstance()->step();
		RenderEngine::getInstance()->step();

		f32 dtMillis = Time::getInstance()->getElapsedTimeMillis();

		if(inverseFPSMillis > dtMillis){
			diff = inverseFPSMillis - dtMillis;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}
		std::cout << 1.0f/Time::getInstance()->getDeltaTimeSeconds() << std::endl;

		Time::getInstance()->endFrame();
	}
}

void Engine::terminate(){
	TRACE();

	ScenesManager::deleteInstance();
	
	terminateSubSystems();

	MaterialManager::deleteInstance();

	ScriptEngine::deleteInstance();
	RenderEngine::deleteInstance();
	EventsManager::getInstance()->terminate();
	EventsManager::deleteInstance();
	TimerManager::deleteInstance();
	Time::deleteInstance();
	Input::deleteInstance();
	RenderContext::terminate();

	Memory::terminate();
}