#pragma once

#include "Core/Core.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"
#include "Config/ConfigObject.hpp"


class GameObject;

CLASS(Scene, ObjectBase)
{
	PRI(SLst(GameObject *), GameObjects, GET);
	PRI(SLst(GameObject *), NewGameObjects, GET);
	PRI(GameObject *, CameraGameObject, GET_SET)

	PRI(f32, Size, GET)
	PRI(SStr, Path, GET)

	PRI(ConfigObject, LoadSceneConfig, NONE)

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
