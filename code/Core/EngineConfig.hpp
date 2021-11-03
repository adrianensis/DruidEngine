#pragma once

#include "Config/ConfigObject.hpp"

class EngineConfig: public ObjectBase, public Singleton<EngineConfig>
{
	GENERATE_METADATA(EngineConfig)
	PRI_M(ConfigObject, Config, GETREF_CONST)

PUB
	void init();
};