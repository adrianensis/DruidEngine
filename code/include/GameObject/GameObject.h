#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "DE_Class.h"
#include "Component.h"

namespace DE {

	template <class K, class V> class HashMap;
	template <class T> class List;
	class Component;
	class Transform;
	class Scene;

// ---------------------------------------------------------------------------

class GameObject : public DE_Class{

private:

	HashMap<ClassId,List<Component*>*>* mComponents;

	Transform* mTransform;

	List<Component*>* getComponents(ClassId classId);

	Scene* mScene;

public:

	DE_CLASS(GameObject, DE_Class);

	void init();

	void addComponent(Component* component, ClassId classId);

	template<class T>
	void addComponent(T* component){
		GameObject::addComponent(component, T::getClassIdStatic());
	}

	template<class T>
	List<T*>* getComponents(){
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassIdStatic()));
	}

	Transform* getTransform() const;
	Scene* getScene() const;
	void setScene(Scene* scene);
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* GAMEOBJECT_H */
