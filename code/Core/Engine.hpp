#pragma once

#include "Core/Core.hpp"

CLASS(Engine, ObjectBase), SINGLETON(Engine) {

	PRI(FPS, NONE, f32)

public:

	~Engine();
	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};