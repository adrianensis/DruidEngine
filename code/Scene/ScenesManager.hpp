#pragma once

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"

namespace DE {
template<class T> class List;
class Scene;
class Script;
class GameObject;

class ScenesManager: public ObjectBase, public Singleton<ScenesManager> {

private:
	List<Scene*>* mScenes;
	PRIVATE(CurrentSceneIndex, NONE, u32)
	
PRIVATE(CurrentScene, NONE, Scene*)
	PRIVATE(SceneHasChanged, NONE, bool)
PRIVATE(GameObjectController, NONE, GameObject*)

	void internalLoadScene();
	void addScene(Scene *newScene);

public:

	GENERATE_METADATA(ScenesManager);

	ScenesManager();
	virtual ~ScenesManager() override;;

	GET(CurrentScene);
	GET(SceneHasChanged);
	GET_SET(GameObjectController);

	void init();
	void step();
	void loadCurrentScene();
	void setScene(u32 i);
};
}

