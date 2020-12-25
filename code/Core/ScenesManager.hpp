#ifndef DE_SCENESMANAGER_H
#define DE_SCENESMANAGER_H

#include "Basic.hpp"
#include "Singleton.hpp"

namespace DE {
template<class T> class List;
class Scene;
class Script;
class GameObject;

class ScenesManager: public DE_Class, public Singleton<ScenesManager> {

private:
	List<Scene*>* mScenes;
	u32 mCurrentSceneIndex;
	Scene* mCurrentScene;
	bool mSceneHasChanged;
	GameObject* mGameObjectController;

	void internalLoadScene();
	void addScene(Scene *newScene);

public:

	DE_CLASS(ScenesManager, DE_Class)

	DE_GET(CurrentScene)
	DE_GET(SceneHasChanged)
	DE_GET_SET(GameObjectController)

	void init();
	void step();
	void loadCurrentScene();
	void setScene(u32 i);
};
}
#endif /* DE_SCENESMANAGER_H */
