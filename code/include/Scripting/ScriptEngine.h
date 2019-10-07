#ifndef DE_SCRIPTENGINE_H
#define DE_SCRIPTENGINE_H

#include "DE_Class.h"
#include "Singleton.h"
#include "List.h"

namespace DE {

class Script;

class ScriptEngine : public DE_Class, public Singleton<ScriptEngine>{
private:

List<Script*>* mScripts;

public:
  DE_CLASS(ScriptEngine, DE_Class);

  void init();
  void addScript(Script* newScript);
  void step();
  void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPTENGINE_H */
