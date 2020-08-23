#ifndef DE_TIME_H
#define DE_TIME_H

#include "BasicTypes.hpp"
#include "Singleton.hpp"
#include "DE_Class.hpp"
#include <chrono>

namespace DE {

//using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Time : public DE_Class, public Singleton<Time>{
private:
	f32 mDeltaTimeMillis;
	f32 mDeltaTimeSeconds;
	std::chrono::milliseconds mDeltaTimeChronoDuration;
	std::chrono::time_point<std::chrono::high_resolution_clock> mInitializationTime, mStartTime, mLastTime;

public:
	DE_CLASS(Time, DE_Class)

	void init();
	void startFrame();
	void endFrame();
	f32 getElapsedTime();
	f32 getDeltaTimeMillis();
	f32 getDeltaTimeSeconds();
};

} /* namespace DE */

#endif /* DE_TIME_H */
