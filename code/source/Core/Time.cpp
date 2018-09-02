#include "Time.h"

namespace DE {

  f32 Time::mDeltaTime = 0.0f;
  time_point Time::mNow, Time::mLastTime;

  Time::Time() = default;

  Time::~Time() = default;

  void Time::init(){
    Time::mDeltaTime = 0.0;
    Time::mLastTime = std::chrono::system_clock::now();
  };

  void Time::tick(){
    Time::mNow = std::chrono::system_clock::now();
    Time::mDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(Time::mNow - Time::mLastTime).count() / 1000.0f;
    Time::mLastTime = std::chrono::system_clock::now();
  };

  f32 Time::getNow(){
      auto epoch = std::chrono::time_point_cast<std::chrono::microseconds>(Time::mNow).time_since_epoch();
      return std::chrono::duration_cast<std::chrono::microseconds>(epoch).count() / 1000.0f;
  };

  f32 Time::getDeltaTime(){
    return Time::mDeltaTime;
  };

  f32 Time::getElapsedTime(){
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Time::mLastTime).count() / 1000.0f;
  };

} /* namespace DE */
