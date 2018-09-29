#include "Engine.h"
#include "Memory.h"
#include "RenderContext.h"
#include "Scene.h"
#include "RenderEngine.h"
#include "ScriptEngine.h"
#include "GameObject.h"

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

  Scene* scene = Memory::allocate<Scene>();
  GameObject* gameObject = Memory::allocate<GameObject>();

  scene->init();
  gameObject->init();

  scene->addGameObject(gameObject);

  mScenes->pushBack(scene);
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
