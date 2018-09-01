#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "RenderEngine.h"

namespace DE {

class Engine {
private:
  f32 fps;

  RenderEngine mRender;

public:
  Engine();
  ~Engine();

  void init();
  void run();
  void terminate();
};

} /* namespace DE */

#endif /* DE_ENGINE_H */
