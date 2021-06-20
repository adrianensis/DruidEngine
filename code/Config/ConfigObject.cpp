#include "Config/ConfigObject.hpp"
#include "File/File.hpp"

void ConfigObject::readFromJsonFile(const std::string &path)
{
	File::readFile(path, [&](std::ifstream &file)
	{
		file >> mJson;
	});
}

void ConfigObject::writeToJsonFile(const std::string &path)
{
	File::writeFile(path, [&](std::ofstream &file)
	{
		file << mJson;
	});
}

void ConfigObject::init()
{
}

void ConfigObject::clear()
{
	mJson.clear();
}

bool ConfigObject::contains(std::string &key) const
{
	return mJson.contains(key);
}