#include "Scripting/ScriptEngine.hpp"

#include "Scripting/Script.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"

#include "Log/Log.hpp"

void ScriptEngine::init() {
	TRACE()

	mController = ScenesManager::getInstance()->getGameObjectController()->getFirstComponent<Script>();
}

void ScriptEngine::addScript(Script *newScript) {
	mScripts.push_back(newScript);
}

void ScriptEngine::step() {

	if (mController) {
		if (!mController->getFirstStepDone()) {
			mController->firstStep();
			mController->firstStepDone();
		}

		mController->step();
	}

	FOR_LIST (it, mScripts) {
		Script* script = *it;

		if (script->isActive()) {
			if (!script->getFirstStepDone()) {
				script->firstStep();
				script->firstStepDone();
			}

			script->step();
		} else if (script->getIsPendingToBeDestroyed()) {
			internalRemoveScript(it);
		}

	}
}

void ScriptEngine::internalRemoveScript(std::list<Script*>::iterator& it) {
	Script* script = *it;
	
	script->terminate();
	script->finallyDestroy();
	delete script;

	it = mScripts.erase(it);
}

void ScriptEngine::terminate() {
	TRACE()

	if(mController){
		mController->terminate();
		delete mController;
	}

	FOR_LIST (it, mScripts) {
		Script* script = *it;
		script->terminate();
		delete script;
	}
}