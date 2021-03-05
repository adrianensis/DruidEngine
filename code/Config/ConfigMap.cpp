#include "Config/ConfigMap.hpp"
#include "File/File.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

String ConfigMap::trueString = "true";
String ConfigMap::falseString = "false";

ConfigMap::ConfigMap() : DE_Class() {
}

ConfigMap::~ConfigMap() {
}

void ConfigMap::readConfigFile(const String &path) {

	File::readFile(path, [&](std::ifstream &file) {

		String lineString;

		String regexVariableNameStr("([a-zA-Z\\-_\\.0-9\\[\\]]+)");
		std::regex regexPair(regexVariableNameStr + "\\s*=\\s*(.*)\\s*");
		std::regex regexComment("\\s*#\\s*");

		while (std::getline(file, lineString)) {

			std::smatch matchPair;
			std::regex_search(lineString, matchPair, regexPair);
			bool isPair = !matchPair.empty();

			std::smatch matchComment;
			std::regex_search(lineString, matchComment, regexComment);
			bool isComment = !matchComment.empty();

			if ((!isComment) && isPair) {
				mMap.set(matchPair[1], matchPair[2]);
				// std::cout << matchPair[1] << " " <<  mMap.get(matchPair[1]) << std::endl;
			}

		}
	});
}

void ConfigMap::writeConfigFile(const String &path) {
	File::writeFile(path, [&](std::ofstream &file) {

		FOR_LIST(it, mMap.getKeys())
		{
			StringRef key = it.get();
			file << key << "=" << mMap.get(key) << std::endl;
		}
	});
}

void ConfigMap::init() {
	mMap.init();
}

void ConfigMap::clear() {
	mMap.clear();
}

bool ConfigMap::contains(StringRef key) {
	return mMap.contains(key);
}

String ConfigMap::getString(const String &key) {
	return mMap.get(key);
}

u32 ConfigMap::getU32(const String &key) {
	return std::stoi(mMap.get(key));
}

f32 ConfigMap::getF32(const String &key) {
	return std::stof(mMap.get(key));
}

bool ConfigMap::getBool(const String &key) {
	return mMap.get(key) == trueString ? true : false;
}

void ConfigMap::setString(const String &key, const String &value) {
	if(!value.empty()){
		mMap.set(key, value);
	}
}

void ConfigMap::setU32(const String &key, u32 value) {
	mMap.set(key, std::to_string(value));
}

void ConfigMap::setF32(const String &key, f32 value) {
	mMap.set(key, std::to_string(value));
}

void ConfigMap::setBool(const String &key, bool value) {
	mMap.set(key, value ? trueString : falseString);
}

}
