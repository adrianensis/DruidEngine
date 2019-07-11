#ifndef DE_SCENE_H
#define DE_SCENE_H

#include "DE_Class.h"

namespace DE {

template <class T> class List;
class GameObject;

class Scene : public DE_Class {

private:

	List<GameObject*>* mGameObjects;
	GameObject* mCamera;

public:

	Scene();
	~Scene() override;

	void init();
	List<GameObject*>* getGameObjects();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);

	void setCamera(GameObject* camera);
	GameObject* getCamera();
};

} /* namespace DE */

#endif /* DE_SCENE_H */
