#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "DE_Class.h"
#include "RenderEngine.h"

namespace DE {

class Engine : public DE_Class{
private:
  f32 fps;

  RenderEngine mRender;

public:
  Engine();
  ~Engine() override;

  void init();
  void run();
  void terminate();
};

} /* namespace DE */

#endif /* DE_ENGINE_H */
