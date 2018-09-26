#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "DE_Class.h"

namespace DE {

class Script : public DE_Class{
private:


public:
  Script();
  ~Script() override;

  void init();
  void update();
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
