#pragma once

#include "Core/ObjectBase.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"

#include <list>

class GameObject;
class ConfigObject;

CLASS(Scene, ObjectBase)
{
private:
	PRI(GameObjects, GET, std::list<GameObject *> *);
	PRI(NewGameObjects, GET, std::list<GameObject *> *);
	PRI(CameraGameObject, GET_SET, GameObject *)

	PRI(Size, GET, f32)
	PRI(Path, GET, std::string)

	PRI(LoadSceneConfigMap, NONE, ConfigObject *)

private:
	void destroyGameObjects();

public:
	Scene();
	~Scene() override;

	void init();

	virtual void serialize(JSON &json) const override;
	virtual void deserialize(const JSON &json) override;
	void loadScene(const std::string &path);
	void saveScene(const std::string &path);

	void unloadScene();

	void addGameObject(GameObject * gameObject);
	void updateComponents(GameObject * gameObject);
	void removeGameObject(GameObject * gameObject);

	void step();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};
