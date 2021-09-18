#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Macros.hpp"

class Log
{
public:
	STC CNS SStr emptyMessage;
	STC std::ofstream logFile;

	Log() = default;
	~Log() = default;

	STC void init();
	STC void terminate();

	STC void log(CNS SStr &str);

	STC void trace(CNS SStr file, u32 line, CNS SStr function, CNS SStr message = emptyMessage);

	STC void echo(CNS SStr &message);

	template <class T>
	STC void var(CNS SStr &varname, T var)
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
	STC void val(CNS T &var)
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

	STC void error(CNS SStr &message);

	STC void brline();
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
