#pragma once

#include "Config/ConfigObject.hpp"

CLASS(EngineConfig, ObjectBase), SINGLETON(EngineConfig)
{
	PRI(ConfigObject, Config, GETREF_CONST)

public:
	void init();
};