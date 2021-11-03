#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Macros.hpp"

class Log
{
PUB
	static const std::string emptyMessage;
	static std::ofstream logFile;

	Log() = default;
	~Log() = default;

	static void init();
	static void terminate();

	static void log(const std::string &str);
    static void append(const std::string &str);

	static void trace(const std::string file, u32 line, const std::string function, const std::string message = emptyMessage);

	static void echo(const std::string &message, bool newLine = true);

    static void custom_echo(const std::string &tag, const std::string &message, bool newLine = true);

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
    static void backspace();
};

#ifdef DE_ENABLE_LOGS
#define TRACE() Log::trace(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define TO_STR(s) #s
#define ECHO(x) Log::echo(x);
#define ECHO_APPEND(x) Log::echo(x, false);
#define CUSTOM_ECHO(Tag, x) Log::custom_echo(Tag, x);
#define CUSTOM_ECHO_APPEND(Tag, x) Log::custom_echo(Tag, x, false);
#define VAR(x) Log::var<REMOVE_POINTER(REMOVE_REF(decltype(x)))>(#x, x);
#define VAL(x) Log::val<REMOVE_POINTER(REMOVE_REF(decltype(x)))>(x);
#define ERROR(x) Log::error(x);
#define BRLINE() Log::brline();
#define BACKSPACE() Log::backspace();
#else
#define TRACE()
#define TO_STR(s)
#define ECHO(x)
#define ECHO_APPEND(x)
#define CUSTOM_ECHO(Tag, x)
#define CUSTOM_ECHO_APPEND(Tag, x)
#define VAR(x)
#define VAL(x)
#define ERROR(x)
#define BRLINE()
#define BACKSPACE()
#endif
