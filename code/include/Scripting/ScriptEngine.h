#ifndef DE_SCRIPTENGINE_H
#define DE_SCRIPTENGINE_H

#include "DE_Class.h"
#include "List.h"

namespace DE {

class Script;

class ScriptEngine : public DE_Class{
private:

List<Script*>* mScripts;

public:
  ScriptEngine();
  ~ScriptEngine() override;

  void init();
  void addScript(Script* newScript);
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPTENGINE_H */
