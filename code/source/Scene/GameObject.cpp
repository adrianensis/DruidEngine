#include "GameObject.h"

#include "List.h"
#include "HashMap.h"

#include "Component.h"
#include "Transform.h"

namespace DE {

// ---------------------------------------------------------------------------

GameObject::GameObject() : DE_Class(){
	mComponentsMap = nullptr;
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
}

GameObject::~GameObject(){
	Memory::free<HashMap<ClassId,List<Component*>*>>(mComponentsMap);
	Memory::free<Transform>(mTransform);
}

// ---------------------------------------------------------------------------

void GameObject::addComponent(Component* component, ClassId classId){
	List<Component*>* list = nullptr;

	//ClassId classId = component->getParentClassId();

	if(! mComponentsMap->contains(classId)){
		list = Memory::allocate<List<Component*>>();
		list->init();
		mComponentsMap->set(classId,list);
	}

	mComponentsMap->get(classId)->pushBack(component);

	component->setGameObject(this);
	component->init();
}

// ---------------------------------------------------------------------------

void GameObject::removeComponent(Component* component, ClassId classId){
	if(mComponentsMap->contains(classId)){
		List<Component*>* list = mComponentsMap->get(classId);
		list->remove(list->find(component));
		component->destroy();
	}
}

// ---------------------------------------------------------------------------

void GameObject::init(){
	// TRACE();

	mComponentsMap = Memory::allocate<HashMap<ClassId,List<Component*>*>>();
	mComponentsMap->init();

	mTransform = Memory::allocate<Transform>();
	//mTransform->init();

	addComponent(mTransform);

	mTag = "";
}

// ---------------------------------------------------------------------------

List<Component*>* GameObject::getComponents(ClassId classId){
	return mComponentsMap->contains(classId) ? mComponentsMap->get(classId) : nullptr;
}

// ---------------------------------------------------------------------------

void GameObject::setIsActive( bool isActive ){
	mIsActive = mIsDestroyed || mIsPendingToBeDestroyed ? false : isActive;

	FOR_LIST(it, mComponentsMap->getValues()){
		FOR_LIST(itComponent, it.get()){
			itComponent.get()->setIsActive(isActive);
		}
	}
};

void GameObject::destroy(){
	mIsPendingToBeDestroyed = true;
	mIsActive = false;

	FOR_LIST(it, mComponentsMap->getKeys()){

		ClassId id = it.get();
		auto list = mComponentsMap->get(id);

		FOR_LIST(itComponent, list){
			removeComponent(itComponent.get(), id);
		}
	}
};

// ---------------------------------------------------------------------------

} /* namespace DE */
