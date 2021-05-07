#pragma once

#include "Core/ObjectBase.hpp"
#include "File/File.hpp"

namespace DE {

template<class T> class List;
class GameObject;
class ConfigMap;

class Scene : public ObjectBase {

private:

	List<GameObject*>* mGameObjects;
	List<GameObject*>* mNewGameObjects;
PRIVATE(CameraGameObject, NONE, GameObject*)

	PRIVATE(Size, NONE, f32)
	PRIVATE(Path, NONE, String)

	void destroyGameObjects();

PRIVATE(LoadSceneConfigMap, NONE, ConfigMap*)
	PRIVATE(MaxGameObjectsToLoadPerFrame, NONE, u32)
	PRIVATE(GameObjectsToLoadTotal, NONE, u32)
	PRIVATE(GameObjectsToLoadIndex, NONE, u32)

public:

	GENERATE_METADATA(Scene);

	Scene();
	virtual ~Scene() override;;

	GET(GameObjects);
	GET(NewGameObjects);
	GET_SET(CameraGameObject);

	GET(Size);
	GET(Path);

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

