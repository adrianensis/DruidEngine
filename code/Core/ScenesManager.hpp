#ifndef DE_SCENESMANAGER_H
#define DE_SCENESMANAGER_H

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"

namespace DE {
template<class T> class List;
class Scene;
class Script;
class GameObject;

class ScenesManager: public DE_Class, public Singleton<ScenesManager> {

private:
	DE_M(Scenes, List<Scene*>*);
	DE_M(CurrentSceneIndex, u32);
	
	DE_M_GET(CurrentScene, Scene*)
	DE_M_GET(SceneHasChanged, bool)
	DE_M_GET_SET(GameObjectController, GameObject*)

	void internalLoadScene();
	void addScene(Scene *newScene);

public:

	DE_CLASS(ScenesManager)



	void init();
	void step();
	void loadCurrentScene();
	void setScene(u32 i);
};
}
#endif /* DE_SCENESMANAGER_H */
