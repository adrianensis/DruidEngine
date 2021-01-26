#include "ScenesManager.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/List.hpp"
#include "Scene/Scene.hpp"
#include "Memory/Memory.hpp"
#include "Scene/GameObject.hpp"
#include "Graphics/Camera.hpp"
#include "Config/EngineConfig.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Scripting/Script.hpp"
#include <string>

namespace DE {

// ---------------------------------------------------------------------------

ScenesManager::ScenesManager() : DE_Class(), Singleton() {
	mCurrentScene = nullptr;
	mSceneHasChanged = true;
}

// ---------------------------------------------------------------------------

ScenesManager::~ScenesManager() {

	FOR_LIST (it, mScenes) {
		DE_FREE(it.get());
	}

	DE_FREE(mScenes);

	mGameObjectController->destroy();
	DE_FREE(mGameObjectController);
}

// ---------------------------------------------------------------------------

void ScenesManager::internalLoadScene() {
	if (EngineConfig::getInstance()->getU32("scenes.length") > 0) {
		std::string sceneName = EngineConfig::getInstance()->getString(
				"scenes[" + std::to_string(mCurrentSceneIndex) + "]");
		mCurrentScene->loadScene(sceneName);
	}

	mGameObjectController->setScene(mCurrentScene);
	RenderEngine::getInstance()->setCamera(mCurrentScene->getCameraGameObject()->getFirstComponent<Camera>());
}

// ---------------------------------------------------------------------------

void ScenesManager::init() {

	DE_INSTANCEABLE_BY_CLASSNAME(GameObject);

	mCurrentSceneIndex = 0;

	mScenes = DE_NEW<List<Scene*>>();
	mScenes->init();

	u32 scenesCount = 1;

	if (EngineConfig::getInstance()->getU32("scenes.length") > 0) {
		scenesCount = EngineConfig::getInstance()->getU32("scenes.length");
	}

	FOR_RANGE(i, 0, scenesCount) {
		Scene* scene = DE_NEW<Scene>();
		scene->init();
		addScene(scene);
	}

	mCurrentScene = mScenes->get(0);
}

// ---------------------------------------------------------------------------

void ScenesManager::step() {

	/*if (mSceneHasChanged) {

	} else {

	}*/

	mCurrentScene->step();
}

void ScenesManager::loadCurrentScene() {
	if (mSceneHasChanged) {
		mCurrentScene = mScenes->get(mCurrentSceneIndex);
		internalLoadScene();
		mSceneHasChanged = false;
	}
}

// ---------------------------------------------------------------------------

void ScenesManager::addScene(Scene *newScene) {
	mScenes->pushBack(newScene);
}

// ---------------------------------------------------------------------------

void ScenesManager::setScene(u32 i) {
	if (mCurrentSceneIndex != i) {
		mCurrentSceneIndex = i;
		mSceneHasChanged = true;
	}
}

// ---------------------------------------------------------------------------

}
