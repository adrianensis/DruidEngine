#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "Component.h"

namespace DE {

class Script : public Component{
private:


public:

  DE_GENERATE_METADATA(Script);

  Script();
  ~Script() override;

  void init();
  void update();
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
