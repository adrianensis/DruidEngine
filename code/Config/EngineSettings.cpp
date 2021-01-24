#include "EngineSettings.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

// ---------------------------------------------------------------------------

EngineSettings::EngineSettings() : DE_Class(), Singleton() {
	mConfigMap = nullptr;
}

// ---------------------------------------------------------------------------

EngineSettings::~EngineSettings() {
	Memory::free<ConfigMap>(mConfigMap);
}

// ---------------------------------------------------------------------------

void EngineSettings::readConfigFile(const std::string &path) {
	mConfigMap->readConfigFile(path);
}

// ---------------------------------------------------------------------------

void EngineSettings::init() {
	mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/engine.conf");
}

// ---------------------------------------------------------------------------

std::string EngineSettings::getString(const std::string &key) {
	return mConfigMap->getString(key);
}

u32 EngineSettings::getU32(const std::string &key) {
	return mConfigMap->getU32(key);
}

f32 EngineSettings::getF32(const std::string &key) {
	return mConfigMap->getF32(key);
}

bool EngineSettings::getBool(const std::string &key) {
	return mConfigMap->getBool(key);
}

// ---------------------------------------------------------------------------
}
