#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Core/BasicTypes.hpp"
#include "Core/Macros.hpp"

class Log
{
public:
	static const std::string emptyMessage;
	static std::ofstream logFile;

	Log() = default;
	~Log() = default;

	static void init();
	static void terminate();

	static void log(const std::string &str);

	static void trace(const std::string file, u32 line, const std::string function, const std::string message = emptyMessage);

	static void echo(const std::string &message);

	template <class T>
	static void var(const std::string &varname, T var)
	{
		std::string valueStr;
		if constexpr (std::is_same<T, std::string>::value)
		{
			valueStr = var;
		}
		else
		{
			valueStr = std::to_string(var);
		}

		log("VAR > " + varname + " : " + valueStr);
	};

	template <class T>
	static void val(const T &var)
	{
		std::string valueStr;
		if constexpr (std::is_same<T, std::string>::value)
		{
			valueStr = var;
		}
		else
		{
			valueStr = std::to_string(var);
		}

		log("VAL > " + valueStr);
	};

	static void error(const std::string &message);

	static void brline();
};

#ifdef DE_ENABLE_LOGS
#define TRACE() Log::trace(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define TO_STR(s) #s
#define ECHO(x) Log::echo(x);
#define VAR(x) Log::var<REMOVE_POINTER(REMOVE_REF(decltype(x)))>(#x, x);
#define VAL(x) Log::val<REMOVE_POINTER(REMOVE_REF(decltype(x)))>(x);
#define ERROR(x) Log::error(x);
#define BRLINE() Log::brline();
#else
#define TRACE()
#define TO_STR(s)
#define ECHO(x)
#define VAR(x)
#define VAL(x)
#define ERROR(x)
#define BRLINE()
#endif
