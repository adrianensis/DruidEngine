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

	FOR_LIST (it, mGameObjects){
		Memory::free<GameObject>(it.get());
	}

	Memory::free<List<GameObject*>>(mGameObjects);
	Memory::free<List<GameObject*>>(mNewGameObjects);
}

// ---------------------------------------------------------------------------

void Scene::init() {
	TRACE();

	mGameObjects = Memory::allocate<List<GameObject*>>();
	mGameObjects->init();

	mNewGameObjects = Memory::allocate<List<GameObject*>>();
	mNewGameObjects->init();

	mMaxNewGameObjectsToSpawn = 20; // TODO ; move to Settings.
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
	FOR_LIST (itGameObjects, mNewGameObjects){
		mGameObjects->pushBack(itGameObjects.get());
	}

	mNewGameObjects->clear();
}

// ---------------------------------------------------------------------------

bool Scene::thereAreNewGameObjects() const { return mNewGameObjects->getLength() > 0; }

// ---------------------------------------------------------------------------

} /* namespace DE */
