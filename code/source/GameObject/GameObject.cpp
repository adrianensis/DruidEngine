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

void GameObject::init() {
	mComponents = Memory::allocate<HashMap<ClassId,List<Component*>*>>();
	mComponents->init();

	mTransform = Memory::allocate<Transform>();
	//mTransform->init();

	addComponent(mTransform);
}

// ---------------------------------------------------------------------------

List<Component*>* GameObject::getComponents(ClassId classId) {
	return mComponents->contains(classId) ? mComponents->get(classId) : nullptr;
}

Transform* GameObject::getTransform(){ return mTransform; }

// ---------------------------------------------------------------------------

} /* namespace DE */
