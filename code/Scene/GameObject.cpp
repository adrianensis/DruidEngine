#include "Scene/GameObject.hpp"

#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"
#include "Config/ConfigMap.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Graphics/Renderer.hpp"

namespace DE {

// ---------------------------------------------------------------------------

GameObject::GameObject() : DE_Class() {
	mComponentsMap = nullptr;
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
	mShouldPersist = false;
}

GameObject::~GameObject() {
	DE_FREE(mComponentsMap);
	DE_FREE(mCacheComponentsFirst);
	DE_FREE(mTransform);
	DE_FREE(mCacheComponentsLists);
}

void GameObject::setCacheLists(ClassId classId, List<Component*>* components) {
	mLastClassIdList = classId;
	mCacheComponentsLists->set(classId, components);
}

void GameObject::setCacheFirst(ClassId classId, Component* component) {
	mLastClassIdFirst = classId;
	mCacheComponentsFirst->set(classId, component);
}

void GameObject::tryCleanCache(ClassId classId) {
	if(mLastClassIdFirst == classId) {
		mCacheComponentsFirst->set(classId, nullptr);
	}

	if(mLastClassIdList == classId) {
		mCacheComponentsLists->set(classId, nullptr);
	}
}

bool GameObject::checkCacheFirst(ClassId classId) {
	return mLastClassIdFirst == classId && mCacheComponentsFirst->get(classId);
}

bool GameObject::checkCacheLists(ClassId classId) {
	return mLastClassIdList == classId && mCacheComponentsLists->contains(classId);
}

// ---------------------------------------------------------------------------

void GameObject::addComponent(Component *component, ClassId classId) {
	List<Component*>* list = nullptr;

	if (!mComponentsMap->contains(classId)) {
		list = DE_NEW<List<Component*>>();
		list->init();
		mComponentsMap->set(classId, list);
	}

	mComponentsMap->get(classId)->pushBack(component);

	component->setGameObject(this);
	component->init();

	tryCleanCache(classId);
}

// ---------------------------------------------------------------------------

void GameObject::removeComponent(Component *component, ClassId classId) {
	if (mComponentsMap->contains(classId) && ! component->getIsPendingToBeDestroyed() && ! component->getIsDestroyed()) {
		List<Component*>* list = mComponentsMap->get(classId);
		list->remove(list->find(component));
		component->destroy();

		tryCleanCache(classId);
	}
}

// ---------------------------------------------------------------------------

void GameObject::init() {
	// TRACE();

	mComponentsMap = DE_NEW<ComponentsMap>();
	mComponentsMap->init();

	mCacheComponentsFirst = DE_NEW<CacheComponentsMap>();
	mCacheComponentsFirst->init();

	mCacheComponentsLists = DE_NEW<ComponentsMap>();
	mCacheComponentsLists->init();

	mTransform = DE_NEW<Transform>();
	addComponent(mTransform);

	mTag = "";
}

// ---------------------------------------------------------------------------

List<Component*>* GameObject::getComponents(ClassId classId) {
	List<Component*>* components = nullptr;

	if(checkCacheLists(classId)) {
		components = mCacheComponentsLists->get(classId);
	} else {
		if(mComponentsMap->contains(classId)) {
			components = mComponentsMap->get(classId);

			if(components->isEmpty()){
				components = nullptr;
			}
		}
	}

	setCacheLists(classId, components);

	return components;
}

// ---------------------------------------------------------------------------

Component* GameObject::getFirstComponent(ClassId classId) {
	Component* component = nullptr;

	if(checkCacheFirst(classId)) {
		component = mCacheComponentsFirst->get(classId);
	} else {
		List<Component*>* components = getComponents(classId);

		if(components && !components->isEmpty()) {
			component = components->getFirst().get();
		}
	}

	setCacheFirst(classId, component);

	return component;
}

// ---------------------------------------------------------------------------

void GameObject::setIsActive(bool isActive) {
	mIsActive = mIsDestroyed || mIsPendingToBeDestroyed ? false : isActive;

	FOR_LIST(it, mComponentsMap->getValues()) {
		FOR_LIST(itComponent, it.get())
		{
			itComponent.get()->setIsActive(isActive);
		}
	}
};

void GameObject::destroy() {
	mIsPendingToBeDestroyed = true;
	mIsActive = false;

	EventOnDestroy event;
	DE_SEND_EVENT(this, this, event);

	onDestroy();

	FOR_LIST(it, mComponentsMap->getKeys()) {

		ClassId id = it.get();
		auto list = mComponentsMap->get(id);

		FOR_LIST(itComponent, list)
		{
			itComponent.get()->setGameObject(nullptr);
			itComponent.get()->setIsActive(false);
			removeComponent(itComponent.get(), id);
		}

		DE_FREE(list);
	}

	mComponentsMap->clear();
};

// ---------------------------------------------------------------------------

void GameObject::save(ConfigMap* configMap, const std::string& objectName) {

	configMap->setString(objectName + ".class", getClassName());

	configMap->setBool(objectName + ".isStatic", getIsStatic());
	configMap->setBool(objectName + ".shouldPersist", getShouldPersist());

	configMap->setString(objectName + ".tag", getTag());

	Transform *t = getTransform();
	Vector3 worldPosition = t->getWorldPosition();
	Vector3 scale = t->getScale();

	configMap->setF32(objectName + ".worldPosition.x", worldPosition.x);
	configMap->setF32(objectName + ".worldPosition.y", worldPosition.y);
	configMap->setF32(objectName + ".size.width", scale.x);
	configMap->setF32(objectName + ".size.height", scale.y);

	// TODO : this property is not needed.
	Collider *collider = getFirstComponent<Collider>();
	configMap->setBool(objectName + ".hasCollider", collider ? true : false);

	FOR_LIST(it, mComponentsMap->getKeys()) {

		ClassId id = it.get();
		FOR_LIST(itComponent, mComponentsMap->get(id))
		{
			itComponent.get()->save(configMap, objectName);
		}
	}
}

void GameObject::load(ConfigMap* configMap, const std::string& objectName) {
	setIsStatic(configMap->getBool(objectName + ".isStatic"));
	setShouldPersist(configMap->getBool(objectName + ".shouldPersist"));

	setTag(configMap->getString(objectName + ".tag"));

	Vector2 worldPosition(configMap->getF32(objectName + ".worldPosition.x"),
			configMap->getF32(objectName + ".worldPosition.y"));

	Vector2 size(configMap->getF32(objectName + ".size.width"), configMap->getF32(objectName + ".size.height"));
	
	getTransform()->setLocalPosition(Vector3(worldPosition.x, worldPosition.y, 0));
	getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = DE_NEW<Renderer>();
	addComponent<Renderer>(renderer);
	renderer->load(configMap, objectName);

	if(configMap->getBool(objectName + ".hasCollider")) {
		RigidBody* rigidBody = DE_NEW<RigidBody>();
		addComponent<RigidBody>(rigidBody);

		Collider* collider = DE_NEW<Collider>();
		addComponent<Collider>(collider);
		collider->load(configMap, objectName);
	}
}

} /* namespace DE */
