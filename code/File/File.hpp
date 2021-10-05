#pragma once

#include "Core/Core.hpp"


CLASS(File, ObjectBase)
{
public:
	STC void init();
	STC void readFile(CNS SStr &path, SFun(void(std::ifstream & file)) callback);
	STC void writeFile(CNS SStr &path, SFun(void(std::ofstream & file)) callback);
};