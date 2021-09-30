#pragma once

#include "Core/Core.hpp"


class Scene;
class Script;
class GameObject;

CLASS(ScenesManager, ObjectBase), SINGLETON(ScenesManager)
{
	PRI(SVec(Scene *), Scenes, NONE);
	PRI(u32, CurrentSceneIndex, NONE)

	PRI(Scene *, CurrentScene, GET)
	PRI(bool, SceneHasChanged, GET)
	PRI(GameObject *, GameObjectController, GET_SET)

	void internalLoadScene();
	void addScene(Scene * newScene);

public:
	ScenesManager();
	~ScenesManager() OVR;

	void init();
	void update();
	void loadCurrentScene();
	void setScene(u32 i);
};