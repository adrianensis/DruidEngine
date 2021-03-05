#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Core/BasicTypes.hpp"
#include "Core/DE_Class.hpp"

namespace DE {

class Log: public DE_Class {
public:

	static const String emptyMessage;
	static std::ofstream logFile;

	Log();
	~Log();

	static void init();
	static void terminate();

	static void log(const String &str);

	static void trace(const String file, u32 line, const String function, const String message =
			emptyMessage);

	static void echo(const String &message);

	template<class T>
	static void var(const String &varname, T var) {
		// std::cout << "DE_VAR > " << varname << " : " << var << std::endl;
		String varStr;
		if constexpr (std::is_same<T, String>::value) {
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

	static void error(const String &message);

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

}

