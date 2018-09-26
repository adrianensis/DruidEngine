#include "GameObject.h"

namespace DE {

GameObject::GameObject() : DE_Class(),
	mComponents(nullptr)
{
}

GameObject::~GameObject() {
	Memory::free<HashMap<ClassId,List<Component*>*>>(mComponents);
}

void GameObject::addComponent(ClassId classId, Component* component) {
	List<Component*>* list = mComponents->get(classId);

	if(list != nullptr){
		list = Memory::allocate<List<Component*>>();
		list->init();
		mComponents->set(classId,list);
	}

	list->pushBack(component);
}

List<Component*>* GameObject::getComponents(ClassId classId) {
	return mComponents->get(classId);
}

void GameObject::init() {
	mComponents = Memory::allocate<HashMap<ClassId,List<Component*>*>>();
	mComponents->init();
}

} /* namespace DE */
