#include "Settings.h"
#include "HashMap.h"
#include "List.h"
#include "Memory.h"
#include <regex>

namespace DE{

// ---------------------------------------------------------------------------

Settings::Settings() : DE_Class(), Singleton(){
  mSettingsMap = nullptr;
}

// ---------------------------------------------------------------------------

Settings::~Settings(){

}

// ---------------------------------------------------------------------------

void Settings::readConfigFile(const std::string& path){

  File::readFile(path, [&](std::ifstream& file) {

    // std::cout << std::endl;
    // std::cout << "Reading config : " << path << std::endl;

    std::string lineString;

    std::regex regexPair("([a-zA-Z]+[a-zA-Z-_\\.0-9]*)\\s*=\\s*(.*)");

    while(std::getline(file,lineString)){

      std::smatch matchPair;

      std::regex_search(lineString, matchPair, regexPair);

      if(!matchPair.empty()){

        mSettingsMap->set(matchPair[1], matchPair[2]);
        // std::cout << matchPair[1] << " ";
        // std::cout << mSettingsMap->get(matchPair[1]) << " ";
      }

      // std::cout << std::endl;
    }
  });
}

// ---------------------------------------------------------------------------

void Settings::writeConfigFile(const std::string& path, HashMap<std::string,std::string>* settinsMap){
  File::writeFile(path, [&](std::ofstream& file) {

    // std::cout << std::endl;
    // std::cout << "Writing config : " << path << std::endl;

    const List<std::string>* keys = settinsMap->getKeys();

    FOR_LIST(it, keys){
      const std::string& key = it.get();

      file << key << "=" << settinsMap->get(key) << std::endl;

      // std::cout << key << " ";
      // std::cout << settinsMap->get(key) << " ";
      // std::cout << std::endl;
    }
  });
}

// ---------------------------------------------------------------------------

void Settings::init(){
  mSettingsMap = Memory::allocate<HashMap<std::string, std::string>>();
	mSettingsMap->init();

  readConfigFile("config/engine.conf");
  readConfigFile("config/test.conf");
  writeConfigFile("config/testOut.conf", mSettingsMap);
}

// ---------------------------------------------------------------------------

const std::string& Settings::getString(const std::string& key){
  return mSettingsMap->get(key);
}

u32 Settings::getU32(const std::string& key){
  return std::stoi(mSettingsMap->get(key));
}

f32 Settings::getF32(const std::string& key){
  return std::stof(mSettingsMap->get(key));
}


// ---------------------------------------------------------------------------
}
