#pragma once

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"
#include "ConfigMap.hpp"

namespace DE {

class EngineConfig: public DE_Class, public Singleton<EngineConfig> {

private:
	DE_M(ConfigMap, ConfigMap)

public:

	DE_CLASS(EngineConfig)

	void init();
	void readConfigFile(const std::string &path);
	std::string getString(const std::string &key);
	u32 getU32(const std::string &key);
	f32 getF32(const std::string &key);
	bool getBool(const std::string &key);
};
}

