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
	PRI(SMap(ClassId, SLst(Component *) *), ComponentsMap, NONE)
	PRI(bool, IsActive, NONE)

	PRI(Scene *, Scene, GET_SET)
	PRI(bool, IsStatic, GET_SET)
	PRI(Transform *, Transform, GET)
	PRI(SStr, Tag, GET_SET)
	PRI(bool, IsPendingToBeDestroyed, GET)
	PRI(bool, IsDestroyed, GET)
	PRI(bool, ShouldPersist, GET_SET)

private:
	CNS SLst(Component *) *getComponents(ClassId classId) CNS;
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
	CNS SLst(T *) *getComponents() CNS
	{
		return reinterpret_cast<CNS SLst(T *> *)(GameObject::getComponents(T::getClassIdStatic()));
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