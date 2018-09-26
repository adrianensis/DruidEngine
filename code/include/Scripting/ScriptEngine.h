#ifndef DE_SCRIPTENGINE_H
#define DE_SCRIPTENGINE_H

#include "DE_Class.h"

namespace DE {

class ScriptEngine : public DE_Class{
private:


public:
  ScriptEngine();
  ~ScriptEngine() override;

  void init();
  void update();
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPTENGINE_H */
