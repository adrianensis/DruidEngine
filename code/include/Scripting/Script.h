#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "Component.h"

namespace DE {

class Script : public Component{
private:

  f32 x = 0;
public:

  DE_CLASS(Script, Component);

  virtual void init();
  virtual void update();
  virtual void step();
  virtual void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
