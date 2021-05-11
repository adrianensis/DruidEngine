#pragma once

#include "Core/Core.hpp"
#include <map>

CLASS(ConfigMap, ObjectBase) {

	PRI(Map, NONE, std::map<std::string, std::string>);

private:
	static std::string trueString;
	static std::string falseString;

public:

	void init();
	void clear();
	bool contains(std::string& key);
	void readConfigFile(const std::string& path);
	void writeConfigFile(const std::string& path);
	std::string getString(const std::string& key);
	u32 getU32(const std::string& key);
	f32 getF32(const std::string& key);
	bool getBool(const std::string& key);
	void setString(const std::string& key, const std::string& value);
	void setU32(const std::string& key, u32 value);
	void setF32(const std::string& key, f32 value);
	void setBool(const std::string& key, bool value);

	/*template<class T>
	const List<T>* getList(const std::string& key);*/
};
