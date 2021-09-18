#pragma once

#include "Core/Core.hpp"
#include "Core/Serialization.hpp"

/*
	example for constant config object:

	ConfigObject CNS conf;
	conf.setF32("key", 1.0f);
*/

CLASS(ConfigObject, ObjectBase)
{
	PRI(Json, GETREF_CONST_SET, JSON);

public:
	void init();
	void clear();
	bool contains(SStr & key) CNS;

	void readFromJsonFile(CNS SStr &path);
	void writeToJsonFile(CNS SStr &path);

	CNS JSON &at(CNS SStr &key) CNS { return mJson.at(key); }
};