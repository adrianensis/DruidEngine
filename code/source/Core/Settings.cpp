#include "Settings.h"
#include "HashMap.h"
#include "List.h"
#include "Memory.h"

namespace DE{

// ---------------------------------------------------------------------------

Settings::Settings() : DE_Class(), Singleton(){
  mConfigMap = nullptr;
}

// ---------------------------------------------------------------------------

Settings::~Settings(){
  // TODO : REMOVE CONFIG MAP
}

// ---------------------------------------------------------------------------

void Settings::readConfigFile(const std::string& path){
  mConfigMap->readConfigFile(path);
}

// ---------------------------------------------------------------------------

void Settings::init(){
  mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

  mConfigMap->readConfigFile("config/engine.conf");
  mConfigMap->readConfigFile("config/test.conf");
}

// ---------------------------------------------------------------------------

const std::string& Settings::getString(const std::string& key){
  return mConfigMap->getString(key);
}

u32 Settings::getU32(const std::string& key){
  return mConfigMap->getU32(key);
}

f32 Settings::getF32(const std::string& key){
  return mConfigMap->getF32(key);
}

bool Settings::getBool(const std::string& key){
  return mConfigMap->getBool(key);
}

// ---------------------------------------------------------------------------
}
