#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "Component.h"

namespace DE {

class Script : public Component{
private:
  bool mFirstStepDone;

public:

  DE_CLASS(Script, Component);

  virtual void init();
  virtual void firstStep();
  virtual void step();
  virtual void terminate();

  bool isFirstStepDone() const { return mFirstStepDone; }
  void firstStepDone() {  mFirstStepDone = true; }
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
