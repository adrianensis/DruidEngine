#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "Memory.h"

namespace DE {

class Engine {
private:
  f32 fps;

public:
  Engine();
  ~Engine();

  void init();
  void run();
  void terminate();
};

} /* namespace DE */

#endif /* DE_ENGINE_H */
