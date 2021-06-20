#pragma once

#include "Config/ConfigObject.hpp"

CLASS(EngineConfig, ObjectBase), SINGLETON(EngineConfig)
{
	PRI(Config, GETREF_CONST, ConfigObject)

public:
	void init();
};