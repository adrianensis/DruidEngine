#pragma once

#include "Core/Core.hpp"


class Scene;
class Script;
class GameObject;

class ScenesManager: public ObjectBase, public Singleton<ScenesManager>
{
	GENERATE_METADATA(ScenesManager)
	PRI std::vector<Scene *> mScenes;
	PRI u32 mCurrentSceneIndex = 0;

	PRI Scene* mCurrentScene = nullptr; GET(CurrentScene)
	PRI bool mSceneHasChanged = false; GET(SceneHasChanged)
	PRI GameObject* mGameObjectController = nullptr; GET_SET(GameObjectController)

	void internalLoadScene();
	void addScene(Scene * newScene);

PUB
	ScenesManager();
	~ScenesManager() override;

	void init();
	void update();
	void loadCurrentScene();
	void setScene(u32 i);
};