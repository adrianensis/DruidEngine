#ifndef DE_FILE_H_
#define DE_FILE_H_

#include "DE_Class.h"

#include <iostream>
#include <fstream>
#include <string>

namespace DE {

class File : public DE_Class{

public:

	DE_CLASS(File, DE_Class);

	static void init();
	static void readFile(const std::string& path);
};

} /* namespace DE */

#endif /* DE_FILE_H_ */
