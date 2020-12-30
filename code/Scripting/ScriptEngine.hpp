#ifndef DE_SCRIPTENGINE_H
#define DE_SCRIPTENGINE_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "List.hpp"

namespace DE {

class Script;
class Iterator;

class ScriptEngine: public DE_Class, public Singleton<ScriptEngine> {
private:

	DE_M(Scripts, List<Script*>*)
	DE_M(Controller, Script*)
	void internalRemoveScript(const Iterator *it);

public:DE_CLASS(ScriptEngine, DE_Class)

	void init();
	void addScript(Script *newScript);
	void step();
	void terminate();
};

} /* namespace DE */

#endif /* DE_SCRIPTENGINE_H */
