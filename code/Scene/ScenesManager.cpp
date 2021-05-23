#include "ScenesManager.hpp"
#include "Scene/Scene.hpp"
#include "Scene/GameObject.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Core/EngineConfig.hpp"
//#include "Scripting/Script.hpp"
#include <string>

ScenesManager::ScenesManager() {
	mCurrentScene = nullptr;
	mSceneHasChanged = true;
}

ScenesManager::~ScenesManager() {

	FOR_LIST (it, mScenes) {
		delete *it;
	}

	mGameObjectController->destroy();
	delete mGameObjectController;
}

void ScenesManager::internalLoadScene() {
	if (EngineConfig::getInstance()->getConfig().getU32("scenes.length") > 0) {
		std::string sceneName = EngineConfig::getInstance()->getConfig().getString(
				"scenes[" + std::to_string(mCurrentSceneIndex) + "]");
		mCurrentScene->loadScene(sceneName);
	}

	mGameObjectController->setScene(mCurrentScene);
	RenderEngine::getInstance()->setCamera(mCurrentScene->getCameraGameObject()->getFirstComponent<Camera>());
}

void ScenesManager::init() {

	/*INSTANCEABLE_BY_CLASSNAME(GameObject);
	INSTANCEABLE_BY_CLASSNAME(MapElement);
	INSTANCEABLE_BY_CLASSNAME(MapElement_Tile);
	INSTANCEABLE_BY_CLASSNAME(MapElement_ActionPoint);*/

	mCurrentSceneIndex = 0;

	u32 scenesCount = 1;

	if (EngineConfig::getInstance()->getConfig().getU32("scenes.length") > 0) {
		scenesCount = EngineConfig::getInstance()->getConfig().getU32("scenes.length");
	}

	FOR_RANGE(i, 0, scenesCount) {
		Scene* scene = new Scene;
		scene->init();
		addScene(scene);
	}

	mCurrentScene = *mScenes.begin();
}

void ScenesManager::step() {
	mCurrentScene->step();
}

void ScenesManager::loadCurrentScene() {
	if (mSceneHasChanged) {
		mCurrentScene = mScenes[mCurrentSceneIndex];
		internalLoadScene();
		mSceneHasChanged = false;
	}
}

void ScenesManager::addScene(Scene *newScene) {
	mScenes.push_back(newScene);
}

void ScenesManager::setScene(u32 i) {
	if (mCurrentSceneIndex != i) {
		mCurrentSceneIndex = i;
		mSceneHasChanged = true;
	}
}