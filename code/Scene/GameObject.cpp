#include "Scene/GameObject.hpp"

#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"

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
	Memory::free<ComponentsMap>(mComponentsMap);
	Memory::free<CacheComponentsMap>(mCacheComponentsFirst);
	Memory::free<Transform>(mTransform);
	Memory::free<ComponentsMap>(mCacheComponentsLists);
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
		list = Memory::allocate<List<Component*>>();
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

	mComponentsMap = Memory::allocate<ComponentsMap>();
	mComponentsMap->init();

	mCacheComponentsFirst = Memory::allocate<CacheComponentsMap>();
	mCacheComponentsFirst->init();

	mCacheComponentsLists = Memory::allocate<ComponentsMap>();
	mCacheComponentsLists->init();

	mTransform = Memory::allocate<Transform>();
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

		Memory::free<List<Component*>>(list);
	}

	mComponentsMap->clear();
};

// ---------------------------------------------------------------------------

} /* namespace DE */
