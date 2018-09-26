#include "Engine.h"
#include "Memory.h"
#include "RenderContext.h"

namespace DE {

Engine::Engine() : DE_Class(),
		mFPS(60),
		mScenes(nullptr),
		mRenderEngine(nullptr),
		mScriptEngine(nullptr)
{
};

Engine::~Engine() = default;

void Engine::init(){
  Memory::init();

  mRenderEngine = Memory::allocate<RenderEngine>();
  mScriptEngine = Memory::allocate<ScriptEngine>();
  mScenes = Memory::allocate<List<Scene*>>();

  mRenderEngine->init();
  mScriptEngine->init();
  mScenes->init();
};

void Engine::run(){
	mRenderEngine->bind();

	while(! RenderContext::isClosed()){
		mScriptEngine->step();

		mRenderEngine->update();
		mRenderEngine->step();
	}

	mScriptEngine->terminate();
	mRenderEngine->terminate();
};

void Engine::terminate() {

	Memory::free<RenderEngine>(mRenderEngine);
	Memory::free<ScriptEngine>(mScriptEngine);
	Memory::free<List<Scene*>>(mScenes);

	Memory::free();
}

} /* namespace DE */
