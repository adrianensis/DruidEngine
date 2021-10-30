#pragma once

#include "Core/Core.hpp"


CLASS(File, ObjectBase)
{
PUB
	static void init();
	static void readFile(const SStr &path, SFun(void(std::ifstream & file)) callback);
	static void writeFile(const SStr &path, SFun(void(std::ofstream & file)) callback);
};