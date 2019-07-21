#include "Engine.h"
#include "Memory.h"
#include "List.h"
#include "Singleton.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "Scene.h"
#include "RenderEngine.h"
#include "ScriptEngine.h"
#include "GameObject.h"
#include "Transform.h"
#include "Script.h"
#include "Camera.h"

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

	mRenderEngine->setCamera((Camera*)scene->getCameraGameObject()->getComponents<Camera>()->get(0));

	List<GameObject*>* gameObjects = scene->getGameObjects();

	for (auto itGameObjects = gameObjects->getIterator(); !itGameObjects.isNull(); itGameObjects.next()){
		GameObject* gameObject = itGameObjects.get();

		List<Script*>* ScriptList = gameObject->getComponents<Script>();
		List<Renderer*>* RendererList = gameObject->getComponents<Renderer>();

		Script* script = ScriptList ? ScriptList->get(0) : nullptr;
		Renderer* renderer = RendererList ? RendererList->get(0) : nullptr;

		if(script){
			mScriptEngine->addScript(script);
		}

		if(renderer){
			mRenderEngine->addRenderer(renderer);
		}
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
