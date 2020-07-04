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

	HashMap<ClassId, List<Component*>*> *mComponentsMap;

	Transform *mTransform;

	List<Component*>* getComponents(ClassId classId);

	Scene *mScene;

	bool mIsStatic;
	bool mIsActive;
	bool mIsDestroyed;
	bool mIsPendingToBeDestroyed;
	bool mShouldPersist;

	std::string mTag;

public:

	DE_CLASS(GameObject, DE_Class)
	;

	void init();

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
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(
				T::getClassIdStatic()));
	}

	Scene* getScene() const {
		return mScene;
	}
	;
	void setScene(Scene *newScene) {
		mScene = newScene;
	}
	;
	bool isStatic() const {
		return mIsStatic;
	}
	;
	void setIsStatic(bool newIsStatic) {
		mIsStatic = newIsStatic;
	}
	;
	Transform* getTransform() const {
		return mTransform;
	}
	;

	const std::string& getTag() const {
		return mTag;
	}
	;
	void setTag(const std::string &tag) {
		mTag = tag;
	}
	;

	bool isActive() const {
		return mIsDestroyed || mIsPendingToBeDestroyed ? false : mIsActive;
	}
	;
	void setIsActive(bool isActive);
	bool isPendingToBeDestroyed() const {
		return mIsPendingToBeDestroyed;
	}
	;
	bool isDestroyed() const {
		return mIsDestroyed;
	}
	;
	void setDestroyed() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	}
	;
	void destroy();

	bool shouldPersist() const {
		return mShouldPersist;
	}
	;
	void setShouldPersist(bool shouldPersist) {
		mShouldPersist = shouldPersist;
	}
	;
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* GAMEOBJECT_H */
