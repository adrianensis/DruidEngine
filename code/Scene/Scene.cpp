#include "Scene/Scene.hpp"
#include "Memory/Memory.hpp"
#include "Containers/List.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Config/ConfigMap.hpp"
#include "Config/EngineConfig.hpp"
#include "Graphics/Camera.hpp"
#include "Scripting/Script.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Maths/MathUtils.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <future>         // std::async, std::future

namespace DE {

Scene::Scene() : ObjectBase() {
	mGameObjects = nullptr;
}

Scene::~Scene() {

	destroyGameObjects();

	Memory::free(mGameObjects);
	Memory::free(mNewGameObjects);
	Memory::free(mLoadSceneConfigMap);
}

void Scene::destroyGameObjects() {
	FOR_LIST (it, mGameObjects) {
		if (!it.get()->getIsDestroyed()) {
			it.get()->destroy();
			Memory::free(it.get());
		}
	}

	if(mCameraGameObject){
		Memory::free(mCameraGameObject->getFirstComponent<Camera>());
		mCameraGameObject->destroy();
		Memory::free(mCameraGameObject);
	}
}

void Scene::init() {
	TRACE()

	mGameObjects = Memory::allocate<List<GameObject*>>();
	mGameObjects->init();

	mNewGameObjects = Memory::allocate<List<GameObject*>>();
	mNewGameObjects->init();

	mSize = 0;

	mPath = "config/sceneTmp.conf";

	// CAMERA
	GameObject* cameraGameObject = Memory::allocate<GameObject>();
	cameraGameObject->init();

	cameraGameObject->getTransform()->setLocalPosition(Vector3(0, 0, 0));

	Camera* cameraComponent = Memory::allocate<Camera>();
	cameraGameObject->addComponent<Camera>(cameraComponent);

	/*
	 if (aspect >= 1.0)
	 ortho(-50.0 * aspect, 50.0 * aspect, -50.0, 50.0, 1.0, -1.0);
	 else
	 ortho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, 1.0, -1.0);
	 */
	// f32 aspect = RenderContext::getAspectRatio();
	f32 size = RenderContext::getWindowSize().y;
	cameraComponent->setOrtho(-size, size, -size, size, 1, -1);

	setCameraGameObject(cameraGameObject);

	// SET DEFAULT SIZE
	mSize = EngineConfig::getInstance()->getF32("scene.defaultSize");

	mMaxGameObjectsToLoadPerFrame = 10; // TODO : move to settings
}

void Scene::loadScene(const String &path) {

	if(!mLoadSceneConfigMap){
		mLoadSceneConfigMap = Memory::allocate<ConfigMap>();
		mLoadSceneConfigMap->init();
	}
	else {
		mLoadSceneConfigMap->clear();
	}

	mPath = path;

	//std::future<void> fut = std::async (&ConfigMap::readConfigFile,&mLoadSceneConfigMap,mPath); 
	mLoadSceneConfigMap->readConfigFile(mPath); // TODO: do async / in other thread.
	//fut.wait();

	mSize = mLoadSceneConfigMap->getF32("scene.size");

	if (mSize == 0) {
		mSize = EngineConfig::getInstance()->getF32("scene.defaultSize");
	}

	u32 length = mLoadSceneConfigMap->getU32("objects.length");

	mGameObjectsToLoadTotal = length;
	mGameObjectsToLoadIndex = 0;
}

void Scene::saveScene(const String &path) {

	ConfigMap* configMap = Memory::allocate<ConfigMap>();
	configMap->init();

	f32 maxSize = 0;
	u32 counter = 0;

	FOR_LIST(it, mGameObjects) {
		if (it.get()->getShouldPersist()) {
			// ECHO("SAVE")
			String indexStr = std::to_string(counter);
			String objectName = "objects[" + indexStr + "]";

			Transform *t = it.get()->getTransform();
			Vector3 worldPosition = t->getWorldPosition();
			Vector3 scale = t->getScale();

			f32 maxObjectScale = std::max(std::abs(scale.x), std::abs(scale.y));
			maxSize = std::max(std::max(maxSize, std::abs(worldPosition.x) + maxObjectScale),
							   std::abs(worldPosition.y) + maxObjectScale);

			it.get()->save(configMap, objectName);

			counter++;
		}
	}

	configMap->setU32("objects.length", counter);
	configMap->setF32("scene.size", maxSize * 2.0f);

	configMap->writeConfigFile(path);

	Memory::free(configMap);
}

void Scene::unloadScene() {
	destroyGameObjects();
}

bool Scene::isLoadFinished() const {
	return mGameObjectsToLoadIndex == mGameObjectsToLoadTotal;
}

void Scene::addGameObject(GameObject *gameObject) {
	gameObject->setScene(this);
	mNewGameObjects->pushBack(gameObject);
}

void Scene::updateComponents(GameObject *gameObject) {
	List<Renderer*>* rendererList = gameObject->getComponents<Renderer>();

	Script* script = gameObject->getFirstComponent<Script>();
	RigidBody* rigidBody = gameObject->getFirstComponent<RigidBody>();

	if (script && !script->getAlreadyAddedToEngine()) {
		ScriptEngine::getInstance()->addScript(script);
		script->setAlreadyAddedToEngine(true);
	}

	if (rendererList) {
		FOR_LIST (it, rendererList) {
			if (!it.get()->getAlreadyAddedToEngine()) {
				RenderEngine::getInstance()->addRenderer(it.get());
				it.get()->setAlreadyAddedToEngine(true);
			}
		}
	}

	if (rigidBody) {
		if (!rigidBody->getAlreadyAddedToEngine()) {
			PhysicsEngine::getInstance()->addRigidBody(rigidBody);
			rigidBody->setAlreadyAddedToEngine(true);
		}

		Collider* collider = gameObject->getFirstComponent<Collider>();

		if (collider && !collider->getAlreadyAddedToEngine()) {
			PhysicsEngine::getInstance()->addCollider(rigidBody, collider);
			collider->setAlreadyAddedToEngine(true);
		}
	}
}

void Scene::removeGameObject(GameObject *gameObject) {

	if (!(gameObject->getIsDestroyed() || gameObject->getIsPendingToBeDestroyed())) {
		auto it = mGameObjects->find(gameObject);
		mGameObjects->remove(it);

		gameObject->destroy();
		gameObject->finallyDestroy();
		Memory::free(gameObject);
	}
}

void Scene::step() {

	// TODO : refactor into a private method
	if(mGameObjectsToLoadIndex < mGameObjectsToLoadTotal){
		FOR_RANGE_COND(i, 0, mMaxGameObjectsToLoadPerFrame, mGameObjectsToLoadIndex < mGameObjectsToLoadTotal){
			String indexStr = std::to_string(mGameObjectsToLoadIndex);
			String objectName = "objects[" + indexStr + "]";

			String className = mLoadSceneConfigMap->getString(objectName + ".class");
			
			GameObject* gameObject = Memory::fromClassName<GameObject>(className);
			gameObject->init();
			gameObject->load(mLoadSceneConfigMap, objectName);
			addGameObject(gameObject);
			mGameObjectsToLoadIndex += 1;
		}
	}

	if (thereAreNewGameObjects()) {

		const List<GameObject*>* newGameObjects = getNewGameObjects();
		u32 maxToSpawn = EngineConfig::getInstance()->getF32("scene.maxNewObjectsToSpawn");

		// VAR(f32, newGameObjects->getLength());

		FOR_LIST (itGameObjects, newGameObjects)
		{
			GameObject* gameObject = itGameObjects.get();

			updateComponents(gameObject);
		}

		flushNewGameObjects();
	}
}

void Scene::flushNewGameObjects() {
	FOR_LIST (itGameObjects, mNewGameObjects) {
		mGameObjects->pushBack(itGameObjects.get());
	}

	mNewGameObjects->clear();
}

bool Scene::thereAreNewGameObjects() const {
	return mNewGameObjects->getLength() > 0;
}

}
