#pragma once

#include "Core/Core.hpp"

class Engine: public ObjectBase, public Singleton<Engine>
{
	GENERATE_METADATA(Engine)
	PRI f32 mFPS = {};

PUB
	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};