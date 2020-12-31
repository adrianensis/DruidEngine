#include "Core/Settings.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Settings::Settings() : DE_Class(), Singleton() {
	mConfigMap = nullptr;
}

// ---------------------------------------------------------------------------

Settings::~Settings() {
	Memory::free<ConfigMap>(mConfigMap);
}

// ---------------------------------------------------------------------------

void Settings::readConfigFile(const std::string &path) {
	mConfigMap->readConfigFile(path);
}

// ---------------------------------------------------------------------------

void Settings::init() {
	mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/engine.conf");
}

// ---------------------------------------------------------------------------

std::string Settings::getString(const std::string &key) {
	return mConfigMap->getString(key);
}

u32 Settings::getU32(const std::string &key) {
	return mConfigMap->getU32(key);
}

f32 Settings::getF32(const std::string &key) {
	return mConfigMap->getF32(key);
}

bool Settings::getBool(const std::string &key) {
	return mConfigMap->getBool(key);
}

// ---------------------------------------------------------------------------
}
