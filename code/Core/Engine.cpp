#include "Core/TimeUtils.hpp"
#include "Core/Engine.hpp"
#include "Core/Singleton.hpp"

#include <string>
#include <iostream>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;


/*Engine::Engine() : Singleton() {
	mFPS = 60; // TODO : Settings?
}

Engine::~Engine() = default;*/

Engine::~Engine() {

};

void Engine::init() {
	mFPS = 60;
}

void Engine::initSubsystems() {

}

void Engine::terminateSubSystems() {

}

void Engine::run() {
	Time::getInstance()->init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (false) {

		Time::getInstance()->startFrame();

		f32 dtMillis = (Time::getInstance()->getElapsedTimeMillis());

		if(inverseFPSMillis > dtMillis){
			diff = inverseFPSMillis - dtMillis;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}
		//std::cout << Time::getInstance()->getDeltaTimeSeconds() << std::endl;

		Time::getInstance()->endFrame();
	}
}

void Engine::terminate() {

}