#include "GameObject.h"

#include "List.h"
#include "HashMap.h"

#include "Component.h"
#include "Transform.h"

namespace DE {

// ---------------------------------------------------------------------------

GameObject::GameObject() : DE_Class(){
	mComponents = nullptr;
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
}

GameObject::~GameObject() {
	Memory::free<HashMap<ClassId,List<Component*>*>>(mComponents);
	Memory::free<Transform>(mTransform);
}

// ---------------------------------------------------------------------------

void GameObject::addComponent(Component* component, ClassId classId) {
	List<Component*>* list = nullptr;

	//ClassId classId = component->getParentClassId();

	if(! mComponents->contains(classId)){
		list = Memory::allocate<List<Component*>>();
		list->init();
		mComponents->set(classId,list);
	}

	mComponents->get(classId)->pushBack(component);

	component->setGameObject(this);
	component->init();
}

// ---------------------------------------------------------------------------

void GameObject::removeComponent(Component* component) {
	 component->destroy();
}

// ---------------------------------------------------------------------------

void GameObject::init() {
	// TRACE();

	mComponents = Memory::allocate<HashMap<ClassId,List<Component*>*>>();
	mComponents->init();

	mTransform = Memory::allocate<Transform>();
	//mTransform->init();

	addComponent(mTransform);

	mTag = "";
}

// ---------------------------------------------------------------------------

List<Component*>* GameObject::getComponents(ClassId classId) {
	return mComponents->contains(classId) ? mComponents->get(classId) : nullptr;
}

// ---------------------------------------------------------------------------

void GameObject::setIsActive( bool isActive ) {
	mIsActive = isActive;

	FOR_LIST(it, mComponents->getValues()) {
		FOR_LIST(itComponent, it.get()) {
			itComponent.get()->setIsActive(isActive);
		}
	}
};

void GameObject::destroy() {
	mDestroy = true;
	mIsActive = false;

	FOR_LIST(it, mComponents->getValues()) {
		FOR_LIST(itComponent, it.get()) {
			removeComponent(itComponent.get());
		}
	}
};

// ---------------------------------------------------------------------------

} /* namespace DE */
