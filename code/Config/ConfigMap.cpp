#include "Config/ConfigMap.hpp"
#include "File/File.hpp"

std::string ConfigMap::trueString = "true";
std::string ConfigMap::falseString = "false";

void ConfigMap::readConfigFile(const std::string& path){

	File::readFile(path, [&](std::ifstream &file){

		std::string lineString;

		std::string regexVariableNameStr("([a-zA-Z\\-_\\.0-9\\[\\]]+)");
		std::regex regexPair(regexVariableNameStr + "\\s*=\\s*(.*)\\s*");
		std::regex regexComment("\\s*#\\s*");

		while (std::getline(file, lineString)){

			std::smatch matchPair;
			std::regex_search(lineString, matchPair, regexPair);
			bool isPair = !matchPair.empty();

			std::smatch matchComment;
			std::regex_search(lineString, matchComment, regexComment);
			bool isComment = !matchComment.empty();

			if ((!isComment) && isPair){
				//mMap.set(matchPair[1], matchPair[2]);
				mMap.insert(std::make_pair(matchPair[1], matchPair[2]));
				// std::cout << matchPair[1] << " " <<  mMap.get(matchPair[1]) << std::endl;
			}

		}
	});
}

void ConfigMap::writeConfigFile(const std::string& path){
	File::writeFile(path, [&](std::ofstream &file){

		for(auto it = mMap.begin(); it != mMap.end(); ++it)
		{		
			file << it->first << "=" << it->second << std::endl;
		}
	});
}

void ConfigMap::clear(){
	mMap.clear();
}

bool ConfigMap::contains(std::string& key){
	return mMap.find(key) != mMap.end();
}

std::string ConfigMap::getString(const std::string& key){
	return mMap[key];
}

u32 ConfigMap::getU32(const std::string& key){
	return std::stoi(mMap[key]);
}

f32 ConfigMap::getF32(const std::string& key){
	return std::stof(mMap[key]);
}

bool ConfigMap::getBool(const std::string& key){
	return mMap[key] == trueString ? true : false;
}

void ConfigMap::setString(const std::string& key, const std::string& value){
	if(!value.empty()){
		mMap.insert(std::make_pair(key, value));
	}
}

void ConfigMap::setU32(const std::string& key, u32 value){
	mMap.insert(std::make_pair(key, std::to_string(value)));
}

void ConfigMap::setF32(const std::string& key, f32 value){
	mMap.insert(std::make_pair(key, std::to_string(value)));
}

void ConfigMap::setBool(const std::string& key, bool value){
	mMap.insert(std::make_pair(key, value ? trueString : falseString));
}