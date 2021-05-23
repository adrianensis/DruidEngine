#include "Core/Time/TimeUtils.hpp"
#include "Core/Time/TimerManager.hpp"
#include "Core/Engine.hpp"
#include "Core/Singleton.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/MaterialManager.hpp"

#include <string>
#include <iostream>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

Engine::~Engine(){

};

void Engine::init(){
	mFPS = 60;

	RenderContext::init();

	TimerManager::getInstance()->init();

	MaterialManager::getInstance()->init();
}

void Engine::initSubsystems(){
	//f32 sceneSize = ScenesManager::getInstance()->getCurrentScene()->getSize();
	RenderEngine::getInstance()->init(4000);
}

void Engine::terminateSubSystems(){

	RenderEngine::getInstance()->terminate();

	TimerManager::getInstance()->terminate();
}

void Engine::run(){
	Time::getInstance()->init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	initSubsystems();

	while (!RenderContext::isClosed()){

		Time::getInstance()->startFrame();

		TimerManager::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());
		RenderEngine::getInstance()->step();

		f32 dtMillis = (Time::getInstance()->getElapsedTimeMillis());

		if(inverseFPSMillis > dtMillis){
			diff = inverseFPSMillis - dtMillis;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}
		std::cout << Time::getInstance()->getDeltaTimeSeconds() << std::endl;

		Time::getInstance()->endFrame();
	}
}

void Engine::terminate(){
	TRACE();

	RenderEngine::deleteInstance();
	TimerManager::deleteInstance();
	Time::deleteInstance();

	RenderContext::terminate();
}