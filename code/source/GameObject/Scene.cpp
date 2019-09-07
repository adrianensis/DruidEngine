#include "Scene.h"
#include "Memory.h"
#include "List.h"
#include "GameObject.h"

namespace DE {

// ---------------------------------------------------------------------------

Scene::Scene() : DE_Class(){
	mGameObjects = nullptr;
}

Scene::~Scene() {
	Memory::free<List<GameObject*>>(mGameObjects);
	Memory::free<List<GameObject*>>(mNewGameObjects);
}

// ---------------------------------------------------------------------------

void Scene::init() {
	mGameObjects = Memory::allocate<List<GameObject*>>();
	mGameObjects->init();

	mNewGameObjects = Memory::allocate<List<GameObject*>>();
	mNewGameObjects->init();
}

// ---------------------------------------------------------------------------

void Scene::addGameObject(GameObject* gameObject) {
	gameObject->setScene(this);
	mNewGameObjects->pushBack(gameObject);
}

// ---------------------------------------------------------------------------

void Scene::removeGameObject(GameObject* gameObject) {
	auto it = mGameObjects->find(gameObject);
	mGameObjects->remove(it);
}

// ---------------------------------------------------------------------------

void Scene::flushNewGameObjects(){
	for (auto itGameObjects = mNewGameObjects->getIterator(); !itGameObjects.isNull(); itGameObjects.next()){
		mGameObjects->pushBack(itGameObjects.get());
	}

	mNewGameObjects->clear();
}

// ---------------------------------------------------------------------------

List<GameObject*>* Scene::getGameObjects() const { return mGameObjects; }
List<GameObject*>* Scene::getNewGameObjects() const { return mNewGameObjects; }
void Scene::setCameraGameObject(GameObject* cameraGameObject){ mCameraGameObject = cameraGameObject; }
GameObject* Scene::getCameraGameObject() const { return mCameraGameObject; }
bool Scene::thereAreNewGameObjects() const { return mNewGameObjects->getLength() > 0; }

// ---------------------------------------------------------------------------

} /* namespace DE */
