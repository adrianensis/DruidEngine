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
#include "MapElement/MapElement.hpp"

namespace DE {

ScenesManager::ScenesManager() : ObjectBase(), Singleton() {
	mCurrentScene = nullptr;
	mSceneHasChanged = true;
}

ScenesManager::~ScenesManager() {

	FOR_LIST (it, mScenes) {
		Memory::free(it.get());
	}

	Memory::free(mScenes);

	mGameObjectController->destroy();
	Memory::free(mGameObjectController);
}

void ScenesManager::internalLoadScene() {
	if (EngineConfig::getInstance()->getU32("scenes.length") > 0) {
		String sceneName = EngineConfig::getInstance()->getString(
				"scenes[" + std::to_string(mCurrentSceneIndex) + "]");
		mCurrentScene->loadScene(sceneName);
	}

	mGameObjectController->setScene(mCurrentScene);
	RenderEngine::getInstance()->setCamera(mCurrentScene->getCameraGameObject()->getFirstComponent<Camera>());
}

void ScenesManager::init() {

	INSTANCEABLE_BY_CLASSNAME(GameObject);
	INSTANCEABLE_BY_CLASSNAME(MapElement);
	INSTANCEABLE_BY_CLASSNAME(MapElement_Tile);
	INSTANCEABLE_BY_CLASSNAME(MapElement_ActionPoint);

	mCurrentSceneIndex = 0;

	mScenes = Memory::allocate<List<Scene*>>();
	mScenes->init();

	u32 scenesCount = 1;

	if (EngineConfig::getInstance()->getU32("scenes.length") > 0) {
		scenesCount = EngineConfig::getInstance()->getU32("scenes.length");
	}

	FOR_RANGE(i, 0, scenesCount) {
		Scene* scene = Memory::allocate<Scene>();
		scene->init();
		addScene(scene);
	}

	mCurrentScene = mScenes->get(0);
}

void ScenesManager::step() {
	mCurrentScene->step();
}

void ScenesManager::loadCurrentScene() {
	if (mSceneHasChanged) {
		mCurrentScene = mScenes->get(mCurrentSceneIndex);
		internalLoadScene();
		mSceneHasChanged = false;
	}
}

void ScenesManager::addScene(Scene *newScene) {
	mScenes->pushBack(newScene);
}

void ScenesManager::setScene(u32 i) {
	if (mCurrentSceneIndex != i) {
		mCurrentSceneIndex = i;
		mSceneHasChanged = true;
	}
}

}
