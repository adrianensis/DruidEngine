#include "Scene.h"
#include "Memory.h"
#include "List.h"
#include "GameObject.h"

namespace DE {

Scene::Scene() : DE_Class(),
	mGameObjects(nullptr)
{
}

Scene::~Scene() {
	Memory::free<List<GameObject*>>(mGameObjects);
}

void Scene::init() {
	mGameObjects = Memory::allocate<List<GameObject*>>();
	mGameObjects->init();
}

List<GameObject*>* Scene::getGameObjects() {
	return mGameObjects;
}

void Scene::addGameObject(GameObject* gameObject) {
	mGameObjects->pushBack(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject) {
	auto it = mGameObjects->find(gameObject);
	mGameObjects->remove(it);
}

void Scene::setCamera(GameObject* camera){
	mCamera = camera;
}

GameObject* Scene::getCamera(){
 	return mCamera;
}


} /* namespace DE */
