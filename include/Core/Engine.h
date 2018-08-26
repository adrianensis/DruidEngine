#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "Memory.h"

namespace DE {

class Engine {
private:
  f32 fps;

public:
  Engine ();
  ~Engine ();

  void init();
  void run();
};

} /* namespace DE */

#endif /* DE_ENGINE_H */
