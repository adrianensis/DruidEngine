#pragma once

#include "Core/DE_Class.hpp"
#include "Core/TimeUtils.hpp"
#include "Assert/Assert.hpp"
#include "Core/Singleton.hpp"

namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;

#define DE_PROFILER_TIMEMARK_START() Profiler::getInstance()->timeMarkStart(__PRETTY_FUNCTION__);
#define DE_PROFILER_TIMEMARK_END() Profiler::getInstance()->timeMarkEnd(__PRETTY_FUNCTION__);

class Profiler : public DE_Class, public Singleton<Profiler>{
private:

	using TimeMap = HashMap<String, f32>;
	using TimeMarkMap = HashMap<String, TimeMark*>;

	DE_M(TimeMap, TimeMap*);
	DE_M(TimeMarkMap, TimeMarkMap*);
	DE_M(TotalTime, f32);

	void printResult(String& name, f32 time);

public:
	DE_CLASS_BODY(Profiler)

	void init();
	void step(f32 deltaTime);
	void terminate();

	void timeMarkStart(StringRef name);
	void timeMarkEnd(StringRef name);
};

}

