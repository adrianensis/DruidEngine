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

namespace DE {

// ---------------------------------------------------------------------------

Scene::Scene() : DE_Class() {
	mGameObjects = nullptr;
}

Scene::~Scene() {

	destroyGameObjects();

	Memory::free<List<GameObject*>>(mGameObjects);
	Memory::free<List<GameObject*>>(mNewGameObjects);
}

// ---------------------------------------------------------------------------

void Scene::destroyGameObjects() {
	FOR_LIST (it, mGameObjects) {
		if (!it.get()->getIsDestroyed()) {
			it.get()->destroy();
			Memory::free<GameObject>(it.get());
		}
	}

	if(mCameraGameObject){
		Memory::free<Camera>(mCameraGameObject->getFirstComponent<Camera>());
		mCameraGameObject->destroy();
		Memory::free<GameObject>(mCameraGameObject);
	}
}
// ---------------------------------------------------------------------------

void Scene::init() {
	DE_TRACE()

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
}

// ---------------------------------------------------------------------------

void Scene::loadScene(const std::string &path) {

	mPath = path; // TODO: copy?

	ConfigMap* configMap = Memory::allocate<ConfigMap>();
	configMap->init();
	configMap->readConfigFile(mPath);

	mSize = configMap->getF32("scene.size");

	if (mSize == 0) {
		mSize = EngineConfig::getInstance()->getF32("scene.defaultSize");
	}

	u32 length = configMap->getU32("objects.length");

	FOR_RANGE(i, 0, length) {
		std::string indexStr = std::to_string(i);
		std::string objectName = "objects[" + indexStr + "]";

		std::string className = configMap->getString(objectName + ".class");
		
		GameObject* gameObject = Memory::fromClassName<GameObject>(className);
		gameObject->init();
		gameObject->load(configMap, objectName);
		addGameObject(gameObject);
	}

	Memory::free<ConfigMap>(configMap);
}

// ---------------------------------------------------------------------------

void Scene::saveScene(const std::string &path) {

	ConfigMap* configMap = Memory::allocate<ConfigMap>();
	configMap->init();

	f32 maxSize = 0;
	u32 counter = 0;

	FOR_LIST(it, mGameObjects) {
		if (it.get()->getShouldPersist()) {
			// ECHO("SAVE")
			std::string indexStr = std::to_string(counter);
			std::string objectName = "objects[" + indexStr + "]";

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

	Memory::free<ConfigMap>(configMap);
}

// ---------------------------------------------------------------------------

void Scene::unloadScene() {
	destroyGameObjects();
}

// ---------------------------------------------------------------------------

void Scene::addGameObject(GameObject *gameObject) {
	gameObject->setScene(this);
	mNewGameObjects->pushBack(gameObject);
}

// ---------------------------------------------------------------------------

void Scene::updateComponents(GameObject *gameObject) {
	List<Renderer*>* rendererList = gameObject->getComponents<Renderer>();

	Script* script = gameObject->getFirstComponent<Script>();
	RigidBody* rigidBody = gameObject->getFirstComponent<RigidBody>();

	if (script && !script->getAlreadyAddedToEngine()) {
		ScriptEngine::getInstance()->addScript(script);
		script->setAlreadyAddedToEngine(true);
	}

	if (rendererList) {
		FOR_LIST (it, rendererList)
		{
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

// ---------------------------------------------------------------------------

void Scene::removeGameObject(GameObject *gameObject) {

	if (!gameObject->getIsDestroyed()) {
		auto it = mGameObjects->find(gameObject);
		mGameObjects->remove(it);

		gameObject->destroy();
		gameObject->setDestroyed();
		Memory::free<GameObject>(gameObject);
	}
}

// ---------------------------------------------------------------------------

void Scene::step() {

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
// ---------------------------------------------------------------------------

void Scene::flushNewGameObjects() {
	FOR_LIST (itGameObjects, mNewGameObjects) {
		mGameObjects->pushBack(itGameObjects.get());
	}

	mNewGameObjects->clear();
}

// ---------------------------------------------------------------------------

bool Scene::thereAreNewGameObjects() const {
	return mNewGameObjects->getLength() > 0;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
