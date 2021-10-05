#include "File/File.hpp"
#include "Log/Log.hpp"

void File::init()
{
	TRACE()
}

void File::readFile(CNS SStr &path, SFun(void(std::ifstream &file)) callback)
{
	std::ifstream file;
	file.open(path);

	if (callback)
	{
		callback(file);
	}

	file.close();
}

void File::writeFile(CNS SStr &path, SFun(void(std::ofstream &file)) callback)
{
	std::ofstream file;
	file.open(path);

	if (callback)
	{
		callback(file);
	}

	file.close();
}