#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "Component.h"

namespace DE {

class Script : public Component{
private:


public:

  DE_CLASS(Script);

  void init();
  void update();
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
