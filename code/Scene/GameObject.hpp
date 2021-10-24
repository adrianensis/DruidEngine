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
	const SLst(Component *) *getComponents(ClassId classId) const;
	Component *getFirstComponent(ClassId classId) const;

	void addComponentToSubsystem(Component* component);

public:
	GameObject();
	~GameObject() override;

	virtual void init();

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
	const SLst(T *) *getComponents() const
	{
		return reinterpret_cast<const SLst(T *> *)(GameObject::getComponents(T::getClassIdStatic()));
	}

	template <class T>
	T *getFirstComponent() const
	{
		return dynamic_cast<T *>(GameObject::getFirstComponent(T::getClassIdStatic()));
	}

	bool isActive() const
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

	virtual void onDestroy(){/*TODO: call script onDestroy here??*/};

	SERIALIZE();
	DESERIALIZE();
};