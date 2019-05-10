#include "Engine.h"
#include "Memory.h"
#include "List.h"
#include "Singleton.h"
#include "RenderContext.h"
#include "Scene.h"
#include "RenderEngine.h"
#include "ScriptEngine.h"
#include "GameObject.h"
#include "Transform.h"
#include "Script.h"

#include <string>

namespace DE {

Engine::Engine() : DE_Class(), Singleton(),
		mFPS(60),
		mScenes(nullptr),
		mRenderEngine(nullptr),
		mScriptEngine(nullptr),
		mCurrentSceneIndex(0)
{
};

Engine::~Engine() = default;

void Engine::init(){

  mRenderEngine = Memory::allocate<RenderEngine>();
  mScriptEngine = Memory::allocate<ScriptEngine>();
  mScenes = Memory::allocate<List<Scene*>>();

  mRenderEngine->init();
  mScriptEngine->init();
  mScenes->init();
};

void Engine::addScene(Scene* newScene){
	mScenes->pushBack(newScene);
}

void Engine::setScene(u32 i){
	mCurrentSceneIndex = i;
}

void Engine::loadScene(Scene* scene){
	List<GameObject*>* gameObjects = scene->getGameObjects();

	auto itGameObjects = gameObjects->getIterator();

	for (; !itGameObjects.isNull(); itGameObjects.next()){
		GameObject* gameObject = itGameObjects.get();

		Script* script = (Script*)gameObject->getComponents<Script>()->get(0);

		mScriptEngine->addScript(script);
	}
}

void Engine::run(){

	loadScene(mScenes->get(mCurrentSceneIndex));

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
};

} /* namespace DE */
