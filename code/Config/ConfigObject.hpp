#pragma once

#include "Core/Core.hpp"
#include "Core/Serialization.hpp"

/*
	example for constant config object:

	ConfigObject const conf;
	conf.setF32("key", 1.0f);
*/

class ConfigObject: public ObjectBase
{
    GENERATE_METADATA(ConfigObject)
	PRI_M(JSON, Json, GETREF_CONST_SET);

PUB
	void init();
	void clear();
	bool contains(SStr & key) const;

	void readFromJsonFile(const SStr &path);
	void writeToJsonFile(const SStr &path);

	const JSON &at(const SStr &key) const { return mJson.at(key); }
};