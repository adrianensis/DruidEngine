#include "Config/ConfigMap.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Memory/Memory.hpp"

namespace DE {

std::string ConfigMap::trueString = "true";
std::string ConfigMap::falseString = "false";

// ---------------------------------------------------------------------------

ConfigMap::ConfigMap() : DE_Class() {
	mMap = nullptr;
}

// ---------------------------------------------------------------------------

ConfigMap::~ConfigMap() {
	Memory::free<HashMap<std::string, std::string>>(mMap);
}

// ---------------------------------------------------------------------------

void ConfigMap::readConfigFile(const std::string &path) {

	File::readFile(path, [&](std::ifstream &file) {

		std::string lineString;

		std::string regexVariableNameStr("([a-zA-Z\\-_\\.0-9\\[\\]]+)");
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
				mMap->set(matchPair[1], matchPair[2]);
				// std::cout << matchPair[1] << " " <<  mMap->get(matchPair[1]) << std::endl;
			}

		}
	});
}

// ---------------------------------------------------------------------------

void ConfigMap::writeConfigFile(const std::string &path) {
	File::writeFile(path, [&](std::ofstream &file) {

		FOR_LIST(it, mMap->getKeys())
		{
			const std::string& key = it.get();
			file << key << "=" << mMap->get(key) << std::endl;
		}
	});
}

// ---------------------------------------------------------------------------

void ConfigMap::init() {
	mMap = Memory::allocate<HashMap<std::string, std::string>>();
	mMap->init();
}

// ---------------------------------------------------------------------------

std::string ConfigMap::getString(const std::string &key) {
	return mMap->get(key);
}

u32 ConfigMap::getU32(const std::string &key) {
	return std::stoi(mMap->get(key));
}

f32 ConfigMap::getF32(const std::string &key) {
	return std::stof(mMap->get(key));
}

bool ConfigMap::getBool(const std::string &key) {
	return mMap->get(key) == trueString ? true : false;
}

void ConfigMap::setString(const std::string &key, const std::string &value) {
	mMap->set(key, value);
}

void ConfigMap::setU32(const std::string &key, u32 value) {
	mMap->set(key, std::to_string(value));
}

void ConfigMap::setF32(const std::string &key, f32 value) {
	mMap->set(key, std::to_string(value));
}

void ConfigMap::setBool(const std::string &key, bool value) {
	mMap->set(key, value ? trueString : falseString);
}

// ---------------------------------------------------------------------------
}
