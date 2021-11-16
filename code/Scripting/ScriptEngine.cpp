#include "Scripting/ScriptEngine.hpp"

#include "Scripting/Script.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"

#include "Log/Log.hpp"

#include "Profiler/Profiler.hpp"

void ScriptEngine::init()
{
	TRACE()

	REGISTER_COMPONENT_CLASS_IN_SUBSYSTEM(Script);

	mController = ScenesManager::getInstance().getGameObjectController()->getFirstComponent<Script>();
}

void ScriptEngine::addComponent(Component *component)
{
	if(component->getClassId() == Script::getClassIdStatic())
	{
		Script* script = static_cast<Script*>(component);
		mScripts.push_back(script);
	}
}

void ScriptEngine::update()
{
	PROFILER_TIMEMARK_START()
	
	if (mController)
	{
		if (!mController->getFirstUpdateDone())
		{
			mController->firstUpdate();
			mController->firstUpdateDone();
		}

		mController->update();
	}

	FOR_LIST(it, mScripts)
	{
		Script *script = *it;

		if (script->isActive())
		{
			if (!script->getFirstUpdateDone())
			{
				script->firstUpdate();
				script->firstUpdateDone();
			}

			script->update();
		}
		else if (script->getIsPendingToBeDestroyed())
		{
			internalRemoveScript(it);
		}
	}

	PROFILER_TIMEMARK_END()
}

void ScriptEngine::internalRemoveScript(std::list<Script *>::iterator &it)
{
	Script *script = *it;

	script->terminate();
	script->finallyDestroy();
	DELETE(script);

	it = mScripts.erase(it);
	--it;
}

void ScriptEngine::terminate()
{
	TRACE()

	if (mController)
	{
		mController->terminate();
		DELETE(mController);
	}

	FOR_LIST(it, mScripts)
	{
		Script *script = *it;
		script->terminate();
		DELETE(script);
	}
}