#pragma once

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include "Containers/List.hpp"

namespace DE {

class Script;
class Iterator;

class ScriptEngine: public DE_Class, public Singleton<ScriptEngine> {
private:

	DE_M(Scripts, List<Script*>*)
	DE_M(Controller, Script*)
	void internalRemoveScript(const Iterator *it);

public:DE_CLASS_BODY(ScriptEngine)

	void init();
	void addScript(Script *newScript);
	void step();
	void terminate();
};

}

