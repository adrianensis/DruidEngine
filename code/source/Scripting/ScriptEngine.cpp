#include "ScriptEngine.h"

#include "Memory.h"
#include "Script.h"

#include "Debug.h"

namespace DE {

// ---------------------------------------------------------------------------

ScriptEngine::ScriptEngine() : DE_Class(), Singleton() {
  mScripts = nullptr;
}

ScriptEngine::~ScriptEngine() = default;

// ---------------------------------------------------------------------------

void ScriptEngine::init(){
  mScripts = Memory::allocate<List<Script*>>();
  mScripts->init();
}

// ---------------------------------------------------------------------------

void ScriptEngine::addScript(Script* newScript){
  mScripts->pushBack(newScript);
}

// ---------------------------------------------------------------------------

void ScriptEngine::step(){
  auto it = mScripts->getIterator();

  for (; !it.isNull(); it.next()){
    //ECHO("ENGINE SCRIPT STEP");
    it.get()->step();
  }
}

// ---------------------------------------------------------------------------

void ScriptEngine::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
