#pragma once

#include "Core/Core.hpp"


class Scene;
class Script;
class GameObject;

class ScenesManager: public ObjectBase, public Singleton<ScenesManager>
{
	GENERATE_METADATA(ScenesManager)
	PRI_M(SVec(Scene *), Scenes, NONE);
	PRI_M(u32, CurrentSceneIndex, NONE)

	PRI_M(Scene *, CurrentScene, GET)
	PRI_M(bool, SceneHasChanged, GET)
	PRI_M(GameObject *, GameObjectController, GET_SET)

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