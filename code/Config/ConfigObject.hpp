#pragma once

#include "Core/Core.hpp"
#include <map>
#include "json.hpp"

using JSON = nlohmann::json;

/*
	example for constant config object:

	ConfigObject const conf;
	conf.setF32("key", 1.0f);
*/

CLASS(ConfigObject, ObjectBase){

	PRI(Json, GET, JSON);

public:

	void init();
	void clear();
	bool contains(std::string& key) const;
	void readConfigFile(const std::string& path);
	void writeConfigFile(const std::string& path);
	std::string getString(const std::string& key) const;
	u32 getU32(const std::string& key) const;
	f32 getF32(const std::string& key) const;
	bool getBool(const std::string& key) const;
	void setString(const std::string& key, const std::string& value);
	void setU32(const std::string& key, u32 value);
	void setF32(const std::string& key, f32 value);
	void setBool(const std::string& key, bool value);

	/*template<class T>
	const std::list<T>* getList(const std::string& key);*/
};