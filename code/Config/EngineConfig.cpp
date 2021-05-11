#include "EngineConfig.hpp"
#include "File/File.hpp"

void EngineConfig::readConfigFile(const std::string& path) {
	mConfigMap.readConfigFile(path);
}

void EngineConfig::init() {
	mConfigMap.init();
	mConfigMap.readConfigFile("config/engine.conf");
}

std::string EngineConfig::getString(const std::string& key) {
	return mConfigMap.getString(key);
}

u32 EngineConfig::getU32(const std::string& key) {
	return mConfigMap.getU32(key);
}

f32 EngineConfig::getF32(const std::string& key) {
	return mConfigMap.getF32(key);
}

bool EngineConfig::getBool(const std::string& key) {
	return mConfigMap.getBool(key);
}