#pragma once

#include "Core/Core.hpp"

#include <vector>

class Scene;
class Script;
class GameObject;

CLASS(ScenesManager, ObjectBase), SINGLETON(ScenesManager)
{
	PRI(Scenes, NONE, std::vector<Scene *>);
	PRI(CurrentSceneIndex, NONE, u32)

	PRI(CurrentScene, GET, Scene *)
	PRI(SceneHasChanged, GET, bool)
	PRI(GameObjectController, GET_SET, GameObject *)

	void internalLoadScene();
	void addScene(Scene * newScene);

public:
	ScenesManager();
	~ScenesManager() override;

	void init();
	void update();
	void loadCurrentScene();
	void setScene(u32 i);
};