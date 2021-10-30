#pragma once

#include "Config/ConfigObject.hpp"

CLASS(EngineConfig, ObjectBase), SINGLETON(EngineConfig)
{
	PRI_M(ConfigObject, Config, GETREF_CONST)

PUB
	void init();
};