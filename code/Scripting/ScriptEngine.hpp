#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

class Script;

CLASS(ScriptEngine, ISubsystem), SINGLETON(ScriptEngine)
{
	PRI_M(SLst(Script *), Scripts, NONE);
	PRI_M(Script *, Controller, NONE)

	void internalRemoveScript(SLst(Script *)::iterator & it);

PUB
	void init();
	virtual void addComponent(Component *component) override;
	void update();
	void terminate();
};