#pragma once

#include "Core/Core.hpp"
#include "Scene/Component.hpp"
#include "Events/Event.hpp"
#include "Core/Serialization.hpp"


class Transform;
class Scene;

CLASS_EVENT(EventOnDestroy) {};

CLASS(GameObject, ObjectBase)
{
	PRI(ComponentsMap, NONE, SMap<ClassId, SLst<Component *> *>)
	PRI(IsActive, NONE, bool)

	PRI(Scene, GET_SET, Scene *)
	PRI(IsStatic, GET_SET, bool)
	PRI(Transform, GET, Transform *)
	PRI(Tag, GET_SET, SStr)
	PRI(IsPendingToBeDestroyed, GET, bool)
	PRI(IsDestroyed, GET, bool)
	PRI(ShouldPersist, GET_SET, bool)

private:
	CNS SLst<Component *> *getComponents(ClassId classId) CNS;
	Component *getFirstComponent(ClassId classId) CNS;

	void addComponentToSubsystem(Component* component);

public:
	GameObject();
	~GameObject() OVR;

	VIR void init();

	void addComponent(Component * component, ClassId classId);
	void removeComponent(Component * component, ClassId classId);

	template <class T>
	void addComponent(T * component)
	{
		GameObject::addComponent(component, T::getClassIdStatic());
	}

	template <class T>
	void removeComponent(T * component)
	{
		GameObject::removeComponent(component, T::getClassIdStatic());
	}

	template <class T>
	CNS SLst<T *> *getComponents() CNS
	{
		return reinterpret_cast<CNS SLst<T *> *>(GameObject::getComponents(T::getClassIdStatic()));
	}

	template <class T>
	T *getFirstComponent() CNS
	{
		return dynamic_cast<T *>(GameObject::getFirstComponent(T::getClassIdStatic()));
	}

	bool isActive() CNS
	{
		return (mIsDestroyed || mIsPendingToBeDestroyed) ? false : mIsActive;
	};

	void setIsActive(bool isActive);

	void finallyDestroy()
	{
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	VIR void onDestroy(){/*TODO: call script onDestroy here??*/};

	SERIALIZE();
	DESERIALIZE();
};