#pragma once

#include "Core/Core.hpp"
#include "File/File.hpp"
#include "Core/Serialization.hpp"
#include "Config/ConfigObject.hpp"


class GameObject;

CLASS(Scene, ObjectBase)
{
	PRI(GameObjects, GET, std::list<GameObject *>);
	PRI(NewGameObjects, GET, std::list<GameObject *>);
	PRI(CameraGameObject, GET_SET, GameObject *)

	PRI(Size, GET, f32)
	PRI(Path, GET, std::string)

	PRI(LoadSceneConfig, NONE, ConfigObject)

private:
	void destroyGameObjects();

public:
	Scene();
	~Scene() override;

	void init();

	SERIALIZE();
	DESERIALIZE();
	void loadScene(const std::string &path);
	void saveScene(const std::string &path);

	void unloadScene();

	void addGameObject(GameObject * gameObject);

	void removeGameObject(GameObject * gameObject);

	void update();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};
