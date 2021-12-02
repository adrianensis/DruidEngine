#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/ObjectBase.hpp"
#include "Core/Serialization.hpp"

/*
	example for constant config object:

	ConfigObject const conf;
	conf.setF32("key", 1.0f);
*/

class ConfigObject: public ObjectBase
{
    GENERATE_METADATA(ConfigObject)
	PRI JSON mJson; GETREF_CONST_SET(Json);

PUB
	void init();
	void clear();
	bool contains(std::string & key) const;

	void readFromJsonFile(const std::string &path);
	void writeToJsonFile(const std::string &path);

	const JSON &at(const std::string &key) const { return mJson.at(key); }
};