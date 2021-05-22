#include "File/File.hpp"
#include "Log/Log.hpp"

void File::init(){
	TRACE()
}

void File::readFile(const std::string& path, std::function<void(std::ifstream &file)> callback){

	std::ifstream file;
	file.open(path);

	if (callback){
		callback(file);
	}

	file.close();
}

void File::writeFile(const std::string& path, std::function<void(std::ofstream &file)> callback){

	std::ofstream file;
	file.open(path);

	if (callback){
		callback(file);
	}

	file.close();
}