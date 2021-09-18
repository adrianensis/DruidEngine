#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

class Script;

CLASS(ScriptEngine, ISubsystem), SINGLETON(ScriptEngine)
{
	PRI(Scripts, NONE, SLst<Script *>);
	PRI(Controller, NONE, Script *)

	void internalRemoveScript(SLst<Script *>::iterator & it);

public:
	void init();
	VIR void addComponent(Component *component) OVR;
	void update();
	void terminate();
};