#include "Config/ConfigObject.hpp"
#include "File/File.hpp"

void ConfigObject::readFromJsonFile(const SStr &path)
{
	File::readFile(path, [&](std::ifstream &file)
	{
		file >> mJson;
	});
}

void ConfigObject::writeToJsonFile(const SStr &path)
{
	File::writeFile(path, [&](std::ofstream &file)
	{
		file << mJson.dump(4);
		// TODO : use dump(4) only for debug and release? but dump() for ship version?
	});
}

void ConfigObject::init()
{
}

void ConfigObject::clear()
{
	mJson.clear();
}

bool ConfigObject::contains(SStr &key) const
{
	return mJson.contains(key);
}