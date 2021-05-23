#include "Core/EngineConfig.hpp"

void EngineConfig::init() {
	mConfig.readConfigFile("config/engine.json");
}