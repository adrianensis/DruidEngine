#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "Core/DE_Class.hpp"
#include "Scene/Component.hpp"

namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;
class Component;
class Transform;
class Scene;

// ---------------------------------------------------------------------------

class GameObject: public DE_Class {
private:

	using ComponentsMap = HashMap<ClassId, List<Component*>*>;
	DE_M(ComponentsMap, ComponentsMap*)

	List<Component*>* getComponents(ClassId classId);

	DE_M(IsActive, bool)

	DE_M_GET_SET(Scene, Scene*)
	DE_M_GET_SET(IsStatic, bool)
	DE_M_GET(Transform, Transform*)
	DE_M_GET_SET(Tag, std::string)
	DE_M_GET(IsPendingToBeDestroyed, bool)
	DE_M_GET(IsDestroyed, bool)
	DE_M_GET_SET(ShouldPersist, bool)

public:

	DE_CLASS(GameObject)

	virtual void init();

	void addComponent(Component *component, ClassId classId);
	void removeComponent(Component *component, ClassId classId);

	template<class T>
	void addComponent(T *component) {
		GameObject::addComponent(component, T::getClassIdStatic());
	}

	template<class T>
	void removeComponent(T *component) {
		GameObject::removeComponent(component, T::getClassIdStatic());
	}

	template<class T>
	List<T*>* getComponents() {
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassIdStatic()));
	}

	bool isActive() const {
		return mIsDestroyed || mIsPendingToBeDestroyed ? false : mIsActive;
	};

	void setIsActive(bool isActive);

	void setDestroyed() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	virtual void onDestroy() { /*TODO: call script onDestroy here??*/ };
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* GAMEOBJECT_H */
