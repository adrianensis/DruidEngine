#include "ScriptEngine.h"

#include "Memory.h"
#include "Script.h"

namespace DE {

// ---------------------------------------------------------------------------

ScriptEngine::ScriptEngine() : DE_Class() {
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
    it.get()->step();
  }
}

// ---------------------------------------------------------------------------

void ScriptEngine::update() {
}

// ---------------------------------------------------------------------------

void ScriptEngine::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
