#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

class Script;

CLASS(ScriptEngine, ISubsystem), SINGLETON(ScriptEngine)
{
	PRI(SLst(Script *), Scripts, NONE);
	PRI(Script *, Controller, NONE)

	void internalRemoveScript(SLst(Script *)::iterator & it);

public:
	void init();
	virtual void addComponent(Component *component) override;
	void update();
	void terminate();
};