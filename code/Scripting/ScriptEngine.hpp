#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include <list>

class Script;

CLASS(ScriptEngine, ObjectBase), SINGLETON(ScriptEngine)
{
	PRI(Scripts, NONE, std::list<Script *>);
	PRI(Controller, NONE, Script *)

	void internalRemoveScript(std::list<Script *>::iterator & it);

public:
	void init();
	void addScript(Script * newScript);
	void update();
	void terminate();
};