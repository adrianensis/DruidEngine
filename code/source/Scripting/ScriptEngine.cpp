#include "ScriptEngine.hpp"

#include "Memory.hpp"
#include "Script.hpp"
#include "ScenesManager.hpp"
#include "GameObject.hpp"

#include "Log.hpp"

namespace DE {

// ---------------------------------------------------------------------------

ScriptEngine::ScriptEngine() : DE_Class(), Singleton() {
	mScripts = nullptr;
}

ScriptEngine::~ScriptEngine() = default;

// ---------------------------------------------------------------------------

void ScriptEngine::init() {
	TRACE();

	mScripts = Memory::allocate<List<Script*>>();
	mScripts->init();
}

// ---------------------------------------------------------------------------

void ScriptEngine::addScript(Script *newScript) {
	mScripts->pushBack(newScript);
}

// ---------------------------------------------------------------------------

void ScriptEngine::step() {

	Script* controller = ScenesManager::getInstance()->getGameObjectController()->getComponents<Script>()->get(0);

	if (controller) {
		if (!controller->isFirstStepDone()) {
			controller->firstStep();
			controller->firstStepDone();
		}

		controller->step();
	}

	FOR_LIST (it, mScripts)
	{
		Script* script = it.get();

		if (script->isActive()) {
			if (!script->isFirstStepDone()) {
				script->firstStep();
				script->firstStepDone();
			}

			script->step();
		} else if (it.get()->isPendingToBeDestroyed()) {
			internalRemoveScript(&it);
		}

	}
}

// ---------------------------------------------------------------------------

void ScriptEngine::internalRemoveScript(const Iterator *it) {
	auto castedIt = it->cast<Script*>();
	mScripts->remove(*castedIt);

	Script* script = (*castedIt).get();
	script->setDestroyed();
	Memory::free<Script>(script);
}

// ---------------------------------------------------------------------------

void ScriptEngine::terminate() {
	TRACE();
	if (mScripts) {
		FOR_LIST (it, mScripts)
		{
			Memory::free<Script>(it.get());
		}

		Memory::free<List<Script*>>(mScripts);
	}

}

// ---------------------------------------------------------------------------

} /* namespace DE */
