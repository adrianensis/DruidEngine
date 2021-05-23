#include "Config/ConfigObject.hpp"
#include "File/File.hpp"

void ConfigObject::readConfigFile(const std::string& path){

	File::readFile(path, [&](std::ifstream &file){
		file >> mJson;
	});
}

void ConfigObject::writeConfigFile(const std::string& path){
	File::writeFile(path, [&](std::ofstream &file){
		file << mJson;
	});
}

void ConfigObject::init(){
	
}

void ConfigObject::clear(){
	mJson.clear();
}

bool ConfigObject::contains(std::string& key) const {
	return mJson.contains(key);
}

std::string ConfigObject::getString(const std::string& key) const {
	return mJson[key].get<std::string>();
}

u32 ConfigObject::getU32(const std::string& key) const {
	return mJson[key].get<u32>();
}

f32 ConfigObject::getF32(const std::string& key) const {
	return mJson[key].get<f32>();
}

bool ConfigObject::getBool(const std::string& key) const {
	return mJson[key].get<bool>();
}

void ConfigObject::setString(const std::string& key, const std::string& value){
	mJson[key] = value;
}

void ConfigObject::setU32(const std::string& key, u32 value){
	mJson[key] = value;
}

void ConfigObject::setF32(const std::string& key, f32 value){
	mJson[key] = value;
}

void ConfigObject::setBool(const std::string& key, bool value){
	mJson[key] = value;
}