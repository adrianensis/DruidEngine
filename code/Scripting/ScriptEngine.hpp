#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

class Script;

CLASS(ScriptEngine, ISubsystem), SINGLETON(ScriptEngine)
{
	PRI(Scripts, NONE, std::list<Script *>);
	PRI(Controller, NONE, Script *)

	void internalRemoveScript(std::list<Script *>::iterator & it);

public:
	void init();
	void addComponent(Component *component);
	void update();
	void terminate();
};