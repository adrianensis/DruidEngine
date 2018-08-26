#ifndef DE_TIME_H
#define DE_TIME_H

#include "BasicTypes.h"
#include <chrono>

namespace DE {

using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Time {
private:
  static f32 mDeltaTime;
  static time_point mNow, mLastTime;

public:
  Time () = default;

  ~Time () = default;

  static void init(){
    mDeltaTime = 0.0;
    mLastTime = std::chrono::system_clock::now();
  };

  static void tick(){
    mNow = std::chrono::system_clock::now();
    mDeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(mNow - mLastTime).count();
  };

  static f32 getNow(){
      auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(mNow).time_since_epoch();
      return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
  };

  static f32 getDeltaTime(){
    return mDeltaTime;
  }
};

} /* namespace DE */

#endif /* DE_TIME_H */
