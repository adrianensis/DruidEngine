#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

class Script;

class ScriptEngine: public ISubsystem, public Singleton<ScriptEngine>
{
	GENERATE_METADATA(ScriptEngine)
	PRI std::list<Script *> mScripts = {};
	PRI Script * mController = {};

	void internalRemoveScript(std::list<Script *>::iterator & it);

PUB
	void init();
	virtual void addComponent(Component *component) override;
	void update();
	void terminate();
};