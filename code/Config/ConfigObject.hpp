#pragma once

#include "Core/Core.hpp"
#include "Core/Serialization.hpp"
#include <map>

/*
	example for constant config object:

	ConfigObject const conf;
	conf.setF32("key", 1.0f);
*/

CLASS(ConfigObject, ObjectBase){

	PRI(Json, GETREF_CONST_SET, JSON);

public:

	void init();
	void clear();
	bool contains(std::string& key) const;

	void readFromJsonFile(const std::string& path);
	void writeToJsonFile(const std::string& path);
	
	const JSON& at(const std::string& key) const { mJson.at(key); }
};