#pragma once

#include "Core/DE_Class.hpp"
#include "File/File.hpp"

namespace DE {

template<class T> class List;
class GameObject;
class ConfigMap;

class Scene: public DE_Class {

private:

	DE_M_GET(GameObjects, List<GameObject*>*)
	DE_M_GET(NewGameObjects, List<GameObject*>*)
	DE_M_GET_SET(CameraGameObject, GameObject*)

	DE_M_GET(Size, f32)
	DE_M_GET(Path, String)

	void destroyGameObjects();

	DE_M(LoadSceneConfigMap, ConfigMap*)
	DE_M(MaxGameObjectsToLoadPerFrame, u32)
	DE_M(GameObjectsToLoadTotal, u32)
	DE_M(GameObjectsToLoadIndex, u32)

public:

	DE_CLASS_BODY(Scene)

	void init();

	void loadScene(const String &path);
	void saveScene(const String &path);
	void unloadScene();
	bool isLoadFinished() const;

	void addGameObject(GameObject *gameObject);
	void updateComponents(GameObject *gameObject);
	void removeGameObject(GameObject *gameObject);

	void step();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};

}

