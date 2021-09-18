#pragma once

#include "Core/Core.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"
#include "Config/ConfigObject.hpp"


class GameObject;

CLASS(Scene, ObjectBase)
{
	PRI(GameObjects, GET, SLst<GameObject *>);
	PRI(NewGameObjects, GET, SLst<GameObject *>);
	PRI(CameraGameObject, GET_SET, GameObject *)

	PRI(Size, GET, f32)
	PRI(Path, GET, SStr)

	PRI(LoadSceneConfig, NONE, ConfigObject)

private:
	void destroyGameObjects();

public:
	Scene();
	~Scene() OVR;

	void init();

	SERIALIZE();
	DESERIALIZE();
	void loadScene(CNS SStr &path);
	void saveScene(CNS SStr &path);

	void unloadScene();

	void addGameObject(GameObject * gameObject);

	void removeGameObject(GameObject * gameObject);

	void update();

	bool thereAreNewGameObjects() CNS;
	void flushNewGameObjects();
};
