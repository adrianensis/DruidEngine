#include "GameObject.h"

#include "List.h"
#include "HashMap.h"

#include "Component.h"
#include "Transform.h"

namespace DE {

// ---------------------------------------------------------------------------

GameObject::GameObject() : DE_Class(),
	mComponents(nullptr),
	mTransform(nullptr)
{
}

// ---------------------------------------------------------------------------

GameObject::~GameObject() {
	Memory::free<HashMap<ClassId,List<Component*>*>>(mComponents);
	Memory::free<Transform>(mTransform);
}

// ---------------------------------------------------------------------------

void GameObject::addComponent(ClassId classId, Component* component) {
	List<Component*>* list = nullptr;

	if(! mComponents->contains(classId)){
		list = Memory::allocate<List<Component*>>();
		list->init();
		mComponents->set(classId,list);
	}

	list->pushBack(component);

	component->setGameObject(this);
	component->init();
}

// ---------------------------------------------------------------------------

List<Component*>* GameObject::getComponents(ClassId classId) {
	return mComponents->get(classId);
}

// ---------------------------------------------------------------------------

void GameObject::init() {
	mComponents = Memory::allocate<HashMap<ClassId,List<Component*>*>>();
	mComponents->init();

	mTransform = Memory::allocate<Transform>();
	//mTransform->init();

	addComponent(Transform::getClassId(), mTransform);
}

// ---------------------------------------------------------------------------

Transform* GameObject::getTransform(){
	return mTransform;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
