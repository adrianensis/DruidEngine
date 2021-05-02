#include "EngineConfig.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"
#include "File/File.hpp"

namespace DE {

EngineConfig::EngineConfig() : ObjectBase(), Singleton() {
}

EngineConfig::~EngineConfig() {
}

void EngineConfig::readConfigFile(const String &path) {
	mConfigMap.readConfigFile(path);
}

void EngineConfig::init() {
	mConfigMap.init();
	mConfigMap.readConfigFile("config/engine.conf");
}

String EngineConfig::getString(const String &key) {
	return mConfigMap.getString(key);
}

u32 EngineConfig::getU32(const String &key) {
	return mConfigMap.getU32(key);
}

f32 EngineConfig::getF32(const String &key) {
	return mConfigMap.getF32(key);
}

bool EngineConfig::getBool(const String &key) {
	return mConfigMap.getBool(key);
}

}
