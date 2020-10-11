#ifndef DE_FILE_H_
#define DE_FILE_H_

#include "DE_Class.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <regex>

namespace DE {

class File: public DE_Class {

public:

	DE_CLASS(File, DE_Class);

	static void init();
	static void readFile(const std::string &path, std::function<void(std::ifstream &file)> callback);
	static void writeFile(const std::string &path, std::function<void(std::ofstream &file)> callback);
};

} /* namespace DE */

#endif /* DE_FILE_H_ */
