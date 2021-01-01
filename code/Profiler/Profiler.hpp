#ifndef DE_PROFILER_H
#define DE_PROFILER_H

#include "Core/DE_Class.hpp"
#include "Core/TimeUtils.hpp"
#include "Assert/Assert.hpp"
#include "Core/Singleton.hpp"

namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;

#define DE_TIMEMARK_START() Profiler::getInstance()->timeMarkStart(__PRETTY_FUNCTION__);
#define DE_TIMEMARK_END() Profiler::getInstance()->timeMarkEnd(__PRETTY_FUNCTION__);

class Profiler : public DE_Class, public Singleton<Profiler>{
private:

	using TimeMap = HashMap<std::string, f32>;
	TimeMap* mTimeMap = nullptr;

	using TimeMarkMap = HashMap<std::string, TimeMark*>;
	TimeMarkMap* mTimeMarkMap = nullptr;

	f32 mTotalTime = 0;

	void printResult(std::string& name, f32 time);

public:
	DE_CLASS(Profiler)

	void init();
	void step(f32 deltaTime);
	void terminate();

	void timeMarkStart(const std::string& name);
	void timeMarkEnd(const std::string& name);
};

} /* namespace DE */

#endif /* DE_PROFILER_H */
