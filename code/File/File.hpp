#pragma once

#include "Core/Core.hpp"


CLASS(File, ObjectBase)
{
public:
	static void init();
	static void readFile(const std::string &path, std::function<void(std::ifstream & file)> callback);
	static void writeFile(const std::string &path, std::function<void(std::ofstream & file)> callback);
};