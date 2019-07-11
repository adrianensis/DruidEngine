#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "DE_Class.h"
#include "Component.h"

namespace DE {

	template <class K, class V> class HashMap;
	template <class T> class List;
	class Component;
	class Transform;

// ---------------------------------------------------------------------------

class GameObject : public DE_Class{

private:

	HashMap<ClassId,List<Component*>*>* mComponents;

	Transform* mTransform;

	List<Component*>* getComponents(ClassId classId);

public:

	DE_CLASS(GameObject);

	void init();

	void addComponent(Component* component);

	template<class T>
	List<T*>* getComponents(){
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassIdStatic()));
	}

	Transform* getTransform();
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* GAMEOBJECT_H */
