#pragma once

#include "Core/Core.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"
#include "Config/ConfigObject.hpp"


class GameObject;

class Scene: public ObjectBase
{
    GENERATE_METADATA(Scene)
	PRI_M(SLst(GameObject *), GameObjects, GET);
	PRI_M(SLst(GameObject *), NewGameObjects, GET);
	PRI_M(GameObject *, CameraGameObject, GET_SET)

	PRI_M(f32, Size, GET)
	PRI_M(SStr, Path, GET)

	PRI_M(ConfigObject, LoadSceneConfig, NONE)

PRI
	void destroyGameObjects();

PUB
	Scene();
	~Scene() override;

	void init();

	SERIALIZE();
	DESERIALIZE();
	void loadScene(const SStr &path);
	void saveScene(const SStr &path);

	void unloadScene();

	void addGameObject(GameObject * gameObject);

	void removeGameObject(GameObject * gameObject);

	void update();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};
