#pragma once

#include "Core/DE_Class.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <regex>

namespace DE {

class File: public DE_Class {

public:

	DE_CLASS_BODY(File)

	static void init();
	static void readFile(const String &path, std::function<void(std::ifstream &file)> callback);
	static void writeFile(const String &path, std::function<void(std::ofstream &file)> callback);
};

}

