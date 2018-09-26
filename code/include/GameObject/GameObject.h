#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "DE_Class.h"
#include "Component.h"
#include "HashMap.h"

namespace DE {

class GameObject : public DE_Class{
private:
	HashMap<ClassId,List<Component*>*>* mComponents;

	void addComponent(ClassId classId, Component* component);
	List<Component*>* getComponents(ClassId classId);

public:
	GameObject();
	~GameObject() override;

	void init();

	template<class T>
	void addComponent(Component* component){
		GameObject::addComponent(getClassId<T>(), component);
	}

	template<class T>
	List<T*>* getComponents(){
		return static_cast<List<T*>*>(GameObject::getComponents(getClassId<T>()));
	}
};

} /* namespace DE */

#endif /* GAMEOBJECT_H */
