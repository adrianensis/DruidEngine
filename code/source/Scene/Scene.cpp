#include "Scene.h"
#include "Memory.h"
#include "List.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vector2.h"
#include "Vector3.h"
#include "ConfigMap.h"
#include "MaterialManager.h"

namespace DE {

// ---------------------------------------------------------------------------

Scene::Scene() : DE_Class(){
	mGameObjects = nullptr;
}

Scene::~Scene() {

	FOR_LIST (it, mGameObjects){
		if(!it.get()->isDestroyed()){
			it.get()->destroy();
			Memory::free<GameObject>(it.get());
		}
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

void Scene::loadScene(const std::string& path) {

	ConfigMap* configMap = Memory::allocate<ConfigMap>();
	configMap->init();

	configMap->readConfigFile(path);

	u32 length = configMap->getU32("objects.length");

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	FOR_RANGE(i, 0, length){
		std::string indexStr = std::to_string(i);
		std::string objectStr = "objects["+indexStr+"]";

		Vector2 worldPosition(configMap->getF32(objectStr+".worldPosition.x"), configMap->getF32(objectStr+".worldPosition.y"));

		Vector2 size(configMap->getF32(objectStr+".size.width"), configMap->getF32(objectStr+".size.height"));
		Vector2 textureRegionPosition(configMap->getF32(objectStr+".texture.region.u"), configMap->getF32(objectStr+".texture.region.v"));
		Vector2 textureRegionSize(configMap->getF32(objectStr+".texture.region.width"), configMap->getF32(objectStr+".texture.region.height"));

    GameObject* gameObject = Memory::allocate<GameObject>();
    gameObject->init();

    gameObject->getTransform()->setLocalPosition(Vector3(worldPosition.x,worldPosition.y,0));
    gameObject->getTransform()->setScale(Vector3(size.x,size.y,1));

    Renderer* renderer = Memory::allocate<Renderer>();
    gameObject->addComponent<Renderer>(renderer);

    renderer->setLayer(1);

		renderer->setMesh(Mesh::getRectangle());
    renderer->setMaterial(material);

		renderer->setRegion(textureRegionPosition.x, textureRegionPosition.y, textureRegionSize.x, textureRegionSize.y);

    gameObject->setIsStatic(configMap->getBool(objectStr+".isStatic"));

    addGameObject(gameObject);
	}

	Memory::free<ConfigMap>(configMap);
}

// ---------------------------------------------------------------------------

void Scene::saveScene(const std::string& path) {

	ConfigMap* configMap = Memory::allocate<ConfigMap>();
	configMap->init();

	u32 counter = 0;
	FOR_LIST(it, mGameObjects) {
		if(it.get()->isStatic()) {
			std::string indexStr = std::to_string(counter);
			std::string objectStr = "objects["+indexStr+"]";

			Transform* t = it.get()->getTransform();
			Renderer* renderer = it.get()->getComponents<Renderer>()->get(0);
			Texture* texture = renderer->getMaterial()->getTexture();
			Vector3 worldPosition = t->getWorldPosition();
			Vector3 size = t->getScale();

			configMap->setF32(objectStr+".worldPosition.x", worldPosition.x);
			configMap->setF32(objectStr+".worldPosition.y", worldPosition.y);
			configMap->setF32(objectStr+".size.width", size.x);
			configMap->setF32(objectStr+".size.height", size.y);
			configMap->setString(objectStr+".texture.path", texture->getPath());
			configMap->setF32(objectStr+".texture.region.u", renderer->getRegionPosition().x);
			configMap->setF32(objectStr+".texture.region.v", renderer->getRegionPosition().y);
			configMap->setF32(objectStr+".texture.region.width", renderer->getRegionSize().x);
			configMap->setF32(objectStr+".texture.region.height", renderer->getRegionSize().y);
			configMap->setBool(objectStr+".isStatic", it.get()->isStatic());

			counter++;
		}
	}

	configMap->setU32("objects.length", counter);

	configMap->writeConfigFile(path);

	Memory::free<ConfigMap>(configMap);
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

	if(!gameObject->isDestroyed()){
		gameObject->destroy();
		gameObject->setDestroyed();
		Memory::free<GameObject>(gameObject);
	}
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
