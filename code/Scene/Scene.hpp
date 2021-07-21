#pragma once

#include "Core/ObjectBase.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"

#include <list>

class GameObject;
class ConfigObject;

CLASS(Scene, ObjectBase), public ISerializable
{
private:
	PRI(GameObjects, GET, std::list<GameObject *> *);
	PRI(NewGameObjects, GET, std::list<GameObject *> *);
	PRI(CameraGameObject, GET_SET, GameObject *)

	PRI(Size, GET, f32)
	PRI(Path, GET, std::string)

	PRI(LoadSceneConfigMap, NONE, ConfigObject *)
	PRI(MaxGameObjectsToLoadPerFrame, NONE, u32)
	PRI(GameObjectsToLoadTotal, NONE, u32)
	PRI(GameObjectsToLoadIndex, NONE, u32)

private:
	void destroyGameObjects();

public:
	Scene();
	~Scene() override;

	void init();

	virtual JSON serialize() const override;
	virtual void deserialize(const JSON &json) override;
	void loadScene(const std::string &path);
	void saveScene(const std::string &path);

	void unloadScene();
	bool isLoadFinished() const;

	void addGameObject(GameObject * gameObject);
	void updateComponents(GameObject * gameObject);
	void removeGameObject(GameObject * gameObject);

	void step();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};
