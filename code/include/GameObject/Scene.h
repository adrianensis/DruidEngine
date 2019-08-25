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

public:

	DE_CLASS(Scene, DE_Class);

	void init();
	List<GameObject*>* getGameObjects();
	List<GameObject*>* getNewGameObjects();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);

	void setCameraGameObject(GameObject* cameraGameObject);
	GameObject* getCameraGameObject();

	bool thereAreNewGameObjects();
	void flushNewGameObjects();
};

} /* namespace DE */

#endif /* DE_SCENE_H */
