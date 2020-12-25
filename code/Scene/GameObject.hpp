#ifndef DE_GAMEOBJECT_H
#define DE_GAMEOBJECT_H

#include "DE_Class.hpp"
#include "Component.hpp"

namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;
class Component;
class Transform;
class Scene;

// ---------------------------------------------------------------------------

class GameObject: public DE_Class {

private:

	HashMap<ClassId, List<Component*>*>* mComponentsMap;

	Transform* mTransform;

	List<Component*>* getComponents(ClassId classId);

	Scene* mScene;

	bool mIsStatic;
	bool mIsActive;
	bool mIsDestroyed;
	bool mIsPendingToBeDestroyed;
	bool mShouldPersist;

	std::string mTag;

public:

	DE_CLASS(GameObject, DE_Class)

	DE_GET_SET(Scene)
	DE_GET_SET(IsStatic)
	DE_GET(Transform)
	DE_GET_SET(Tag)
	DE_GET(IsPendingToBeDestroyed)
	DE_GET(IsDestroyed)
	DE_GET_SET(ShouldPersist)

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
