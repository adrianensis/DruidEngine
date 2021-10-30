#pragma once

#include "Core/Core.hpp"

CLASS(Engine, ObjectBase), SINGLETON(Engine)
{
	PRI_M(f32, FPS, NONE)

PUB
	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};