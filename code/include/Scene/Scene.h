#ifndef DE_SCENE_H
#define DE_SCENE_H

#include "DE_Class.h"

namespace DE {

template <class T> class List;
class GameObject;

class Scene : public DE_Class {

private:

	List<GameObject*>* mGameObjects;
	List<GameObject*>* mNewGameObjects;
	GameObject* mCameraGameObject;

	u32 mMaxNewGameObjectsToSpawn;

public:

	DE_CLASS(Scene, DE_Class);

	void init();

	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);

	DE_GET(List<GameObject*>* ,GameObjects);
	DE_GET(List<GameObject*>* ,NewGameObjects);
	DE_GET(GameObject* ,CameraGameObject);
	DE_SET(GameObject*, CameraGameObject);

	DE_GET(u32, MaxNewGameObjectsToSpawn);

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};

} /* namespace DE */

#endif /* DE_SCENE_H */
