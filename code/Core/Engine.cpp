#include "Core/Time/TimeUtils.hpp"
#include "Core/Time/TimerManager.hpp"
#include "Core/Engine.hpp"
#include "Core/Singleton.hpp"
#include "Graphics/RenderContext.hpp"

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
}

void Engine::initSubsystems(){

}

void Engine::terminateSubSystems(){

	TimerManager::getInstance()->terminate();
}

void Engine::run(){
	Time::getInstance()->init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (!RenderContext::isClosed()){

		Time::getInstance()->startFrame();

		TimerManager::getInstance()->step(Time::getInstance()->getDeltaTimeSeconds());

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
	TimerManager::deleteInstance();
	Time::deleteInstance();

	RenderContext::terminate();
}