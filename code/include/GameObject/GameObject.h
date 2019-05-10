#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "DE_Class.h"

namespace DE {

	template <class K, class V> class HashMap;
	template <class T> class List;
	class Component;
	class Transform;

class GameObject : public DE_Class{

private:

	HashMap<ClassId,List<Component*>*>* mComponents;

	Transform* mTransform;

	void addComponent(ClassId classId, Component* component);
	List<Component*>* getComponents(ClassId classId);

public:

	DE_GENERATE_METADATA(GameObject)

	GameObject();
	~GameObject() override;

	void init();

	template<class T>
	void addComponent(Component* component){
		GameObject::addComponent(T::getClassId(), component);
	}

	template<class T>
	List<T*>* getComponents(){
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassId()));
	}

	Transform* getTransform();
};

} /* namespace DE */

#endif /* GAMEOBJECT_H */
