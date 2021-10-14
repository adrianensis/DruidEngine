#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Macros.hpp"

class Log
{
public:
	static const SStr emptyMessage;
	static std::ofstream logFile;

	Log() = default;
	~Log() = default;

	static void init();
	static void terminate();

	static void log(const SStr &str);
    static void append(const SStr &str);

	static void trace(const SStr file, u32 line, const SStr function, const SStr message = emptyMessage);

	static void echo(const SStr &message, bool newLine = true);

	template <class T>
	static void var(const SStr &varname, T var)
	{
		SStr valueStr;
		if constexpr (std::is_same<T, SStr>::value)
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
		SStr valueStr;
		if constexpr (std::is_same<T, SStr>::value)
		{
			valueStr = var;
		}
		else
		{
			valueStr = std::to_string(var);
		}

		log("VAL > " + valueStr);
	};

	static void error(const SStr &message);

	static void brline();
    static void backspace();
};

#ifdef DE_ENABLE_LOGS
#define TRACE() Log::trace(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define TO_STR(s) #s
#define ECHO(x) Log::echo(x);
#define ECHO_APPEND(x) Log::echo(x, false);
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
#define VAR(x)
#define VAL(x)
#define ERROR(x)
#define BRLINE()
#define BACKSPACE()
#endif
