#pragma once

#include "Core/Core.hpp"
#include "Scene/Component.hpp"
#include "Events/Event.hpp"
#include "Core/Serialization.hpp"


class Transform;
class Scene;

class EventOnDestroy: public Event { GENERATE_METADATA(EventOnDestroy) };

class GameObject: public ObjectBase
{
    GENERATE_METADATA(GameObject)
	PRI std::map<ClassId, std::list<Component *> *> mComponentsMap;
	PRI bool mIsActive = false;

	PRI Scene* mScene = nullptr; GET_SET(Scene)
	PRI bool mIsStatic = false; GET_SET(IsStatic)
	PRI Transform* mTransform = nullptr; GET(Transform)
	PRI std::string mTag; GET_SET(Tag)
	PRI bool mIsPendingToBeDestroyed = false; GET(IsPendingToBeDestroyed)
	PRI bool mIsDestroyed = false; GET(IsDestroyed)
	PRI bool mShouldPersist = false; GET_SET(ShouldPersist)

PRI
	const std::list<Component *> *getComponents(ClassId classId) const;
	Component *getFirstComponent(ClassId classId) const;

	void addComponentToSubsystem(Component* component);

PUB
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
	const std::list<T *> *getComponents() const
	{
		return reinterpret_cast<const std::list<T *> *>(GameObject::getComponents(T::getClassIdStatic()));
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