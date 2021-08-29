#pragma once

#include "Core/Core.hpp"
#include "Scene/Component.hpp"
#include "Events/Event.hpp"
#include "Core/Serialization.hpp"

#include <list>
#include <map>

class Transform;
class Scene;

CLASS_EVENT(EventOnDestroy) {};

CLASS(GameObject, ObjectBase)
{
	PRI(ComponentsMap, NONE, std::map<ClassId, std::list<Component *> *>)
	PRI(IsActive, NONE, bool)

	PRI(Scene, GET_SET, Scene *)
	PRI(IsStatic, GET_SET, bool)
	PRI(Transform, GET, Transform *)
	PRI(Tag, GET_SET, std::string)
	PRI(IsPendingToBeDestroyed, GET, bool)
	PRI(IsDestroyed, GET, bool)
	PRI(ShouldPersist, GET_SET, bool)

private:
	const std::list<Component *> *getComponents(ClassId classId) const;
	Component *getFirstComponent(ClassId classId) const;

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

	virtual void serialize(JSON &json) const override;
	virtual void deserialize(const JSON &json) override;
};