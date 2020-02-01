#include "ScriptEngine.h"

#include "Memory.h"
#include "Script.h"

#include "Log.h"

namespace DE {

// ---------------------------------------------------------------------------

ScriptEngine::ScriptEngine() : DE_Class(), Singleton() {
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
  FOR_LIST (it, mScripts){
    //ECHO("ENGINE SCRIPT STEP");
    it.get()->step();
  }
}

// ---------------------------------------------------------------------------

void ScriptEngine::terminate() {
  TRACE();
  FOR_LIST (it, mScripts){
		Memory::free<Script>(it.get());
	}

  Memory::free<List<Script*>>(mScripts);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
