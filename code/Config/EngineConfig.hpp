#pragma once

#include "Core/Core.hpp"
#include "ConfigMap.hpp"

CLASS(EngineConfig, ObjectBase), SINGLETON(EngineConfig) {

private:
	PRI(ConfigMap, NONE, ConfigMap)

public:

	void init();
	void readConfigFile(const std::string& path);
	std::string getString(const std::string& key);
	u32 getU32(const std::string& key);
	f32 getF32(const std::string& key);
	bool getBool(const std::string& key);
};