#pragma once

#include "Core/DE_Class.hpp"
#include "Scene/Component.hpp"
#include "Events/Event.hpp"
#include "Config/ConfigMap.hpp"
namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;
class Component;
class Transform;
class Scene;

DE_EVENT_DECLARATION_BEGIN(EventOnDestroy)
DE_EVENT_DECLARATION_END(EventOnDestroy)

class GameObject: public DE_Class {
private:

	using ComponentsMap = HashMap<ClassId, List<Component*>*>;
	DE_M(ComponentsMap, ComponentsMap*)

	List<Component*>* getComponents(ClassId classId);
	Component* getFirstComponent(ClassId classId);

	DE_M(IsActive, bool)

	DE_M_GET_SET(Scene, Scene*)
	DE_M_GET_SET(IsStatic, bool)
	DE_M_GET(Transform, Transform*)
	DE_M_GET_SET(Tag, String)
	DE_M_GET(IsPendingToBeDestroyed, bool)
	DE_M_GET(IsDestroyed, bool)
	DE_M_GET_SET(ShouldPersist, bool)

public:

	DE_CLASS_BODY(GameObject)

	virtual void init();

	void addComponent(Component* component, ClassId classId);
	void removeComponent(Component* component, ClassId classId);

	template<class T>
	void addComponent(T* component) {
		GameObject::addComponent(component, T::getClassIdStatic());
	}

	template<class T>
	void removeComponent(T* component) {
		GameObject::removeComponent(component, T::getClassIdStatic());
	}

	template<class T>
	List<T*>* getComponents() {
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassIdStatic()));
	}

	template<class T>
	T* getFirstComponent() {
		return dynamic_cast<T*>(GameObject::getFirstComponent(T::getClassIdStatic()));
	}

	bool isActive() const {
		return (mIsDestroyed || mIsPendingToBeDestroyed) ? false : mIsActive;
	};

	void setIsActive(bool isActive);

	void finallyDestroy() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	virtual void onDestroy() { /*TODO: call script onDestroy here??*/ };

	virtual void save(ConfigMap* configMap, StringRef objectName);
	virtual void load(ConfigMap* configMap, StringRef objectName);
};

}

