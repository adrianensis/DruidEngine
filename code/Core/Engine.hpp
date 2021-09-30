#pragma once

#include "Core/Core.hpp"

CLASS(Engine, ObjectBase), SINGLETON(Engine)
{
	PRI(f32, FPS, NONE)

public:
	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};