#ifndef DE_SCENE_H
#define DE_SCENE_H

#include "DE_Class.h"
#include "List.h"
#include "GameObject.h"

namespace DE {

class Scene : public DE_Class {
private:
	List<GameObject*>* mGameObjects;

public:
	Scene();
	~Scene() override;

	void init();
	List<GameObject*>* getGameObjects();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
};

} /* namespace DE */

#endif /* DE_SCENE_H */
