#include "Time.h"

namespace DE {

  f32 Time::mDeltaTime = 0.0f;
  time_point Time::mNow, Time::mLastTime;

  Time::Time() = default;

  Time::~Time() = default;

  void Time::init(){
    Time::mDeltaTime = 0.0;
    Time::mLastTime = std::chrono::high_resolution_clock::now();
  };

  void Time::tick(){
    Time::mNow = std::chrono::high_resolution_clock::now();
    Time::mDeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Time::mLastTime).count();
    Time::mLastTime = std::chrono::high_resolution_clock::now();
  };

  f32 Time::getNow(){
      auto epoch = Time::mNow.time_since_epoch();
      return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
  };

  f32 Time::getDeltaTime(){
    return Time::mDeltaTime;
  };

  f32 Time::getElapsedTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Time::mLastTime).count();
  };

} /* namespace DE */
