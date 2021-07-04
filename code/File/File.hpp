#pragma once

#include "Core/ObjectBase.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

CLASS(File, ObjectBase)
{
public:
	static void init();
	static void readFile(const std::string &path, std::function<void(std::ifstream & file)> callback);
	static void writeFile(const std::string &path, std::function<void(std::ofstream & file)> callback);
};