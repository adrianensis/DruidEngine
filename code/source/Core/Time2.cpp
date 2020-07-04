#include "Time2.hpp"
#include "Log.hpp"

namespace DE {

f32 Time::mDeltaTimeMillis = 0.0f;
f32 Time::mDeltaTimeSeconds = 0.0f;
std::chrono::time_point<std::chrono::steady_clock> Time::mNow, Time::mLastTime;

// ---------------------------------------------------------------------------

Time::Time() = default;

Time::~Time() = default;

// ---------------------------------------------------------------------------

void Time::init() {
	TRACE();

	Time::mDeltaTimeMillis = 0.0;
	Time::mDeltaTimeSeconds = 0.0;
	Time::mLastTime = std::chrono::steady_clock::now();
}

// ---------------------------------------------------------------------------

void Time::tick() {
	Time::mNow = std::chrono::steady_clock::now();
	Time::mDeltaTimeMillis = std::chrono::duration_cast<
			std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - Time::mLastTime).count();
	Time::mDeltaTimeSeconds = Time::mDeltaTimeMillis / 1000.0f;
	Time::mLastTime = std::chrono::steady_clock::now();
}

// ---------------------------------------------------------------------------

f32 Time::getNow() {
	auto epoch = Time::mNow.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
}

// ---------------------------------------------------------------------------

f32 Time::getDeltaTimeMillis() {
	return Time::mDeltaTimeMillis;
}

// ---------------------------------------------------------------------------

f32 Time::getDeltaTimeSeconds() {
	return Time::mDeltaTimeSeconds;
}

// ---------------------------------------------------------------------------

f32 Time::getElapsedTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - Time::mLastTime).count();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
