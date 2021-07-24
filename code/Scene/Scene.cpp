#include "Scene/Scene.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Config/ConfigObject.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Maths/MathUtils.hpp"
#include "Core/EngineConfig.hpp"
#include <future> // std::async, std::future
#include "Core/ClassManager.hpp"

Scene::Scene()
{
	mGameObjects = nullptr;
}

Scene::~Scene()
{
	destroyGameObjects();

	DELETE(mGameObjects);
	DELETE(mNewGameObjects);

	if (mLoadSceneConfigMap)
	{
		DELETE(mLoadSceneConfigMap);
	}
}

void Scene::destroyGameObjects()
{
	FOR_LIST(it, *mGameObjects)
	{
		if (!(*it)->getIsDestroyed())
		{
			(*it)->destroy();
			DELETE((*it));
		}
	}

	if (mCameraGameObject)
	{
		Camera *cameraComponent = mCameraGameObject->getFirstComponent<Camera>();
		mCameraGameObject->removeComponent<Camera>(cameraComponent);
		DELETE(cameraComponent);
		mCameraGameObject->destroy();
		DELETE(mCameraGameObject);
	}
}

void Scene::init()
{
	TRACE()

	mGameObjects = NEW(std::list<GameObject *>);

	mNewGameObjects = NEW(std::list<GameObject *>);

	mSize = 0;

	mPath = "config/sceneTmp.json";

	// CAMERA
	GameObject *cameraGameObject = NEW(GameObject);
	cameraGameObject->init();

	cameraGameObject->getTransform()->setLocalPosition(Vector3(0, 0, 0));

	Camera *cameraComponent = NEW(Camera);
	cameraGameObject->addComponent<Camera>(cameraComponent);

	f32 size = RenderContext::getWindowSize().y;
	// TODO : use RenderContext::getWindowSize().x also? To keep the scaleproportions?
	cameraComponent->setOrtho(-size, size, -size, size, 1, -1);

	setCameraGameObject(cameraGameObject);

	// SET DEFAULT SIZE
	mSize = EngineConfig::getInstance()->getConfig().at("scene").at("defaultSize").get<f32>();
}

void Scene::saveScene(const std::string &path)
{
	mPath = path;

	ConfigObject configMap;
	configMap.init();

	JSON json;
	serialize(json);

	configMap.setJson(json);

	configMap.writeToJsonFile(path);
}

void Scene::loadScene(const std::string &path)
{
	mPath = path;

	if (!mLoadSceneConfigMap)
	{
		mLoadSceneConfigMap = NEW(ConfigObject);
		mLoadSceneConfigMap->init();
	}
	else
	{
		mLoadSceneConfigMap->clear();
	}

	//std::future<void> fut = std::async (&ConfigObject::readFromJsonFile,&mLoadSceneConfigMap,mPath);
	mLoadSceneConfigMap->readFromJsonFile(mPath); // TODO: do async / in other thread.
	//fut.wait();

	mSize = mLoadSceneConfigMap->at("size").get<f32>();

	if (mSize == 0)
	{
		mSize = EngineConfig::getInstance()->getConfig().at("scene").at("defaultSize").get<f32>();
	}

	deserialize(mLoadSceneConfigMap->getJson());
}

SERIALIZE(Scene)
{
	f32 maxSize = 0;
	
	FOR_LIST(it, *mGameObjects)
	{
		GameObject* obj = *it;
		if(obj)
		{
			if (obj->getShouldPersist())
			{
				Transform *t = obj->getTransform();
				Vector3 worldPosition = t->getWorldPosition();
				Vector3 scale = t->getScale();

				f32 maxObjectScale = std::max(std::abs(scale.x), std::abs(scale.y));
				maxSize = std::max(std::max(maxSize, std::abs(worldPosition.x) + maxObjectScale),
								std::abs(worldPosition.y) + maxObjectScale);
			}
		}
	}

	DO_SERIALIZE_LIST_IF("objects", *mGameObjects, [](GameObject* gameObject)
	{
		return gameObject->getShouldPersist();
	})

	DO_SERIALIZE("size", maxSize * 2.0f)
}

DESERIALIZE(Scene)
{
	DO_DESERIALIZE("size", mSize)

	std::list<GameObject *> tmpList;
	DO_DESERIALIZE_LIST("objects", tmpList, [](const JSON &json)
	{
		GameObject *gameObject = (GameObject*) INSTANCE_BY_NAME(json["class"]);
		gameObject->init();
		return gameObject;
	})

	FOR_LIST(it, tmpList)
	{
		addGameObject(*it);
	}
}

void Scene::unloadScene()
{
	destroyGameObjects();
}

void Scene::addGameObject(GameObject *gameObject)
{
	gameObject->setScene(this);
	mNewGameObjects->push_back(gameObject);
}

void Scene::updateComponents(GameObject *gameObject)
{
	const std::list<Renderer *> *rendererList = gameObject->getComponents<Renderer>();

	/*Script* script = gameObject->getFirstComponent<Script>();
	RigidBody* rigidBody = gameObject->getFirstComponent<RigidBody>();

	if (script && !script->getAlreadyAddedToEngine()) {
		ScriptEngine::getInstance()->addScript(script);
		script->setAlreadyAddedToEngine(true);
	}*/

	if (rendererList)
	{
		FOR_LIST(it, *rendererList)
		{
			if (!(*it)->getAlreadyAddedToEngine())
			{
				RenderEngine::getInstance()->addRenderer((*it));
				(*it)->setAlreadyAddedToEngine(true);
			}
		}
	}

	/*if (rigidBody) {
		if (!rigidBody->getAlreadyAddedToEngine()) {
			PhysicsEngine::getInstance()->addRigidBody(rigidBody);
			rigidBody->setAlreadyAddedToEngine(true);
		}

		Collider* collider = gameObject->getFirstComponent<Collider>();

		if (collider && !collider->getAlreadyAddedToEngine()) {
			PhysicsEngine::getInstance()->addCollider(rigidBody, collider);
			collider->setAlreadyAddedToEngine(true);
		}
	}*/
}

void Scene::removeGameObject(GameObject *gameObject)
{
	if (!(gameObject->getIsDestroyed() || gameObject->getIsPendingToBeDestroyed()))
	{
		mGameObjects->remove(gameObject);

		gameObject->destroy();
		gameObject->finallyDestroy();
		DELETE(gameObject);
	}
}

void Scene::step()
{
	// TODO : refactor into a private method
	/*if (mGameObjectsToLoadIndex < mGameObjectsToLoadTotal)
	{
		FOR_RANGE_COND(i, 0, mMaxGameObjectsToLoadPerFrame, mGameObjectsToLoadIndex < mGameObjectsToLoadTotal)
		{
			std::string className = "GameObject"; //mLoadSceneConfigMap->at("class").get<std::string>();

			GameObject *gameObject = (GameObject*) INSTANCE_BY_NAME(className); //Memory::fromClassName<GameObject>(className));
			gameObject->init();
			gameObject->deserialize(JSON());
			addGameObject(gameObject);
			mGameObjectsToLoadIndex += 1;
		}
	}*/

	if (thereAreNewGameObjects())
	{
		FOR_LIST(itGameObjects, *mNewGameObjects)
		{
			GameObject *gameObject = (*itGameObjects);

			updateComponents(gameObject);
		}

		flushNewGameObjects();
	}
}

void Scene::flushNewGameObjects()
{
	FOR_LIST(itGameObjects, *mNewGameObjects)
	{
		mGameObjects->push_back(*itGameObjects);
	}

	mNewGameObjects->clear();
}

bool Scene::thereAreNewGameObjects() const
{
	return mNewGameObjects->size() > 0;
}