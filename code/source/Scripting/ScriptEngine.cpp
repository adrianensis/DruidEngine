#include "ScriptEngine.h"

#include "Memory.h"
#include "Script.h"
#include "ScenesManager.h"
#include "GameObject.h"

#include "Log.h"

namespace DE {

// ---------------------------------------------------------------------------

ScriptEngine::ScriptEngine() : DE_Class(), Singleton(){
  mScripts = nullptr;
}

ScriptEngine::~ScriptEngine() = default;

// ---------------------------------------------------------------------------

void ScriptEngine::init(){
	TRACE();

  mScripts = Memory::allocate<List<Script*>>();
  mScripts->init();
}

// ---------------------------------------------------------------------------

void ScriptEngine::addScript(Script* newScript){
  mScripts->pushBack(newScript);
}

// ---------------------------------------------------------------------------

void ScriptEngine::step(){

  Script* controller = ScenesManager::getInstance()->getGameObjectController()->getComponents<Script>()->get(0);

  if(! controller->isFirstStepDone()){
    controller->firstStep();
    controller->firstStepDone();
  }

  controller->step();

  FOR_LIST (it, mScripts){
    Script* script = it.get();

    if(! script->isFirstStepDone()){
      script->firstStep();
      script->firstStepDone();
    }

    script->step();
  }
}

// ---------------------------------------------------------------------------

void ScriptEngine::terminate(){
  TRACE();
  if(mScripts){
    FOR_LIST (it, mScripts){
  		Memory::free<Script>(it.get());
  	}
    
    Memory::free<List<Script*>>(mScripts);
  }

}

// ---------------------------------------------------------------------------

} /* namespace DE */
