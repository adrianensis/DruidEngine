#include "Scene/Scene.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
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

}

Scene::~Scene()
{
	destroyGameObjects();
}

void Scene::destroyGameObjects()
{
	FOR_LIST(it, mGameObjects)
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

	mSize = 0;

	mPath = "config/sceneTmp.json";

	// CAMERA
	GameObject *cameraGameObject = NEW(GameObject);
	cameraGameObject->init();

	// cameraGameObject->getTransform()->setLocalPosition(Vector3(0, 0, -100.0f));
	cameraGameObject->getTransform()->setLocalPosition(Vector3(0, 0, 0));

	Camera *cameraComponent = NEW(Camera);
	cameraComponent->init();

	f32 size = RenderContext::getWindowSize().y;
	// TODO : use RenderContext::getWindowSize().x also? To keep the scaleproportions?
	cameraComponent->setOrtho(-size, size, -size, size, 1, -1);
	// cameraComponent->setPerspective(1, 1000, RenderContext::getAspectRatio(), 90);

	cameraGameObject->addComponent<Camera>(cameraComponent);

	setCameraGameObject(cameraGameObject);

	// SET DEFAULT SIZE
	mSize = EngineConfig::getInstance()->getConfig().at("scene").at("defaultSize").get<f32>();

	mLoadSceneConfig.init();
}

void Scene::saveScene(CNS SStr &path)
{
	mPath = path;

	ConfigObject configMap;
	configMap.init();

	JSON json;
	serialize(json);

	configMap.setJson(json);

	configMap.writeToJsonFile(path);
}

void Scene::loadScene(CNS SStr &path)
{
	mPath = path;

	mLoadSceneConfig.clear();

	//std::future<void> fut = std::async (&ConfigObject::readFromJsonFile,&mLoadSceneConfig,mPath);
	mLoadSceneConfig.readFromJsonFile(mPath); // TODO: do async / in other thread.
	//fut.wait();

	mSize = mLoadSceneConfig.at("size").get<f32>();

	if (mSize == 0)
	{
		mSize = EngineConfig::getInstance()->getConfig().at("scene").at("defaultSize").get<f32>();
	}

	deserialize(mLoadSceneConfig.getJson());
}

SERIALIZE_IMPL(Scene)
{
	f32 maxSize = 0;

	FOR_LIST(it, mGameObjects)
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

	DO_SERIALIZE_LIST_IF("objects", mGameObjects, [](GameObject* gameObject)
	{
		return gameObject->getShouldPersist();
	})

	DO_SERIALIZE("size", maxSize * 2.0f)
}

DESERIALIZE_IMPL(Scene)
{ 
	DO_DESERIALIZE("size", mSize)

	if(json.contains("objects"))
	{
		SLst(GameObject *) tmpList;
		DO_DESERIALIZE_LIST("objects", tmpList, [](CNS JSON &json)
		{
			GameObject *gameObject = INSTANCE_BY_NAME(json["class"], GameObject);
			return gameObject;
		})

		FOR_LIST(it, tmpList)
		{
			(*it)->init();
			addGameObject(*it);
		}
	}
}

void Scene::unloadScene()
{
	destroyGameObjects();
}

void Scene::addGameObject(GameObject *gameObject)
{
	gameObject->setScene(this);
	mNewGameObjects.push_back(gameObject);
}

void Scene::removeGameObject(GameObject *gameObject)
{
	if (!gameObject->getIsDestroyed() && !gameObject->getIsPendingToBeDestroyed())
	{
		mGameObjects.remove(gameObject);

		gameObject->destroy();
		gameObject->finallyDestroy();
		DELETE(gameObject);
	}
}

void Scene::update()
{
	// TODO : refactor into a private method
	/*if (mGameObjectsToLoadIndex < mGameObjectsToLoadTotal)
	{
		FOR_RANGE_COND(i, 0, mMaxGameObjectsToLoadPerFrame, mGameObjectsToLoadIndex < mGameObjectsToLoadTotal)
		{
			SStr className = "GameObject"; //mLoadSceneConfig->at("class").get<SStr>();

			GameObject *gameObject = (GameObject*) INSTANCE_BY_NAME(className); //Memory::fromClassName<GameObject>(className));
			gameObject->init();
			gameObject->deserialize(JSON());
			addGameObject(gameObject);
			mGameObjectsToLoadIndex += 1;
		}
	}*/

	if (thereAreNewGameObjects())
	{
		/*FOR_LIST(it, mNewGameObjects)
		{
			updateComponents(*it);
		}*/

		flushNewGameObjects();
	}
}

void Scene::flushNewGameObjects()
{
	FOR_LIST(it, mNewGameObjects)
	{
		mGameObjects.push_back(*it);
	}

	mNewGameObjects.clear();
}

bool Scene::thereAreNewGameObjects() CNS
{
	return mNewGameObjects.size() > 0;
}