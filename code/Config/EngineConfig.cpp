#include "EngineConfig.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

// ---------------------------------------------------------------------------

EngineConfig::EngineConfig() : DE_Class(), Singleton() {
	mConfigMap = nullptr;
}

// ---------------------------------------------------------------------------

EngineConfig::~EngineConfig() {
	Memory::free<ConfigMap>(mConfigMap);
}

// ---------------------------------------------------------------------------

void EngineConfig::readConfigFile(const std::string &path) {
	mConfigMap->readConfigFile(path);
}

// ---------------------------------------------------------------------------

void EngineConfig::init() {
	mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/engine.conf");
}

// ---------------------------------------------------------------------------

std::string EngineConfig::getString(const std::string &key) {
	return mConfigMap->getString(key);
}

u32 EngineConfig::getU32(const std::string &key) {
	return mConfigMap->getU32(key);
}

f32 EngineConfig::getF32(const std::string &key) {
	return mConfigMap->getF32(key);
}

bool EngineConfig::getBool(const std::string &key) {
	return mConfigMap->getBool(key);
}

// ---------------------------------------------------------------------------
}
