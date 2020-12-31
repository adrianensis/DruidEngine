#ifndef DE_LOG_H
#define DE_LOG_H

#include <string>
#include <iostream>
#include <fstream>

#include "Core/BasicTypes.hpp"
#include "Core/DE_Class.hpp"

namespace DE {

class Log: public DE_Class {
public:

	static const std::string emptyMessage;
	static std::ofstream logFile;

	Log();
	~Log();

	static void init();
	static void terminate();

	static void log(const std::string &str);

	static void trace(const std::string file, u32 line, const std::string function, const std::string message =
			emptyMessage);

	static void echo(const std::string &message);

	template<class T>
	static void var(const std::string &varname, T var) {
		// std::cout << "DE_VAR > " << varname << " : " << var << std::endl;
		std::string varStr;
		if constexpr (std::is_same<T, std::string>::value) {
			varStr = var;
		} else {
			varStr = std::to_string(var);
		}

		log("DE_VAR > " + varname + " : " + varStr);
	};

	template<class T>
	static void val(const T &var) {
		// std::cout << "DE_VAL > " << var << std::endl;
		log("DE_VAL > " + std::to_string(var));
	};

	static void error(const std::string &message);

	static void brline();
};

#ifdef DE_ENABLE_LOGS
  #define DE_TRACE() Log::trace(__FILE__, __LINE__, __PRETTY_FUNCTION__);
  #define TO_STR(s) #s
  #define ECHO(x) Log::echo(x);
  #define VAR(T,x) Log::var<T>(#x, x);
  #define VAL(T,x) Log::val<T>(x);
  #define ERROR(x) Log::error(x);
  #define BRLINE() Log::brline();
#else
#define DE_TRACE()
#define TO_STR(s)
#define ECHO(x)
#define VAR(T,x)
#define VAL(T,x)
#define ERROR(x)
#define BRLINE()
#endif

} /* namespace DE */

#endif /* DE_LOG_H */
