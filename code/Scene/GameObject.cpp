#include "Scene/GameObject.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"
#include "Config/ConfigObject.hpp"
#include "Core/ClassManager.hpp"
#include "Graphics/Renderer.hpp"

GameObject::GameObject()
{
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
	mShouldPersist = false;
}

GameObject::~GameObject()
{
	//DELETE(mTransform);
}

void GameObject::addComponent(Component *component, ClassId classId)
{
	if (!MAP_CONTAINS(mComponentsMap, classId))
	{
		MAP_INSERT(mComponentsMap, classId, NEW(std::list<Component *>));
	}

	mComponentsMap.at(classId)->push_back(component);

	component->setGameObject(this);
	component->onComponentAdded();

	ADD_COMPONENT_TO_SUBSYSTEM(component);
}

void GameObject::removeComponent(Component *component, ClassId classId)
{
	if (MAP_CONTAINS(mComponentsMap, classId) && !component->getIsPendingToBeDestroyed() && !component->getIsDestroyed())
	{
		std::list<Component *> *list = mComponentsMap.at(classId);
		list->remove(component);
		component->destroy();
	}
}

void GameObject::init()
{
	// TRACE();

	mTransform = NEW(Transform);
	addComponent(mTransform);

	mTag = "";
}

const std::list<Component *> *GameObject::getComponents(ClassId classId) const
{
	std::list<Component *> *components = nullptr;

	if (MAP_CONTAINS(mComponentsMap, classId))
	{
		components = mComponentsMap.at(classId);

		if (components->empty())
		{
			components = nullptr;
		}
	}

	return components;
}

Component *GameObject::getFirstComponent(ClassId classId) const
{
	Component *component = nullptr;
	const std::list<Component *> *components = getComponents(classId);

	if (components && !components->empty())
	{
		component = components->front();
	}

	return component;
}

void GameObject::setIsActive(bool isActive)
{
	mIsActive = mIsDestroyed || mIsPendingToBeDestroyed ? false : isActive;

	FOR_MAP(it, mComponentsMap)
	{
		FOR_LIST(itComponent, *it->second)
		{
			(*itComponent)->setIsActive(isActive);
		}
	}
}

void GameObject::destroy()
{
	mIsPendingToBeDestroyed = true;
	mIsActive = false;

	EventOnDestroy event;
	SEND_EVENT(this, this, event);

	onDestroy();

	FOR_MAP(it, mComponentsMap)
	{
		auto list = it->second;

		FOR_LIST(itComponent, *list)
		{
			(*itComponent)->setGameObject(nullptr);
			(*itComponent)->destroy();
		}

		DELETE(list);
	}

	mComponentsMap.clear();

	DELETE(mTransform);
}

SERIALIZE_IMPL(GameObject)
{
	//json["id"] = getObjectId();
	DO_SERIALIZE("class", getClassName())

	DO_SERIALIZE("is_static", mIsStatic)
	DO_SERIALIZE("should_persist", mShouldPersist)

	DO_SERIALIZE("tag", mTag)

	DO_SERIALIZE("transform", mTransform)

	FOR_MAP(it, mComponentsMap)
	{
		DO_SERIALIZE_LIST_IF("components", *(it->second), [](Component* component)
		{
			return component->getClassId() != Transform::getClassIdStatic();
		})
	}
}

DESERIALIZE_IMPL(GameObject)
{
	DO_DESERIALIZE("is_static", mIsStatic)
	DO_DESERIALIZE("should_persist", mShouldPersist)

	DO_DESERIALIZE("tag", mTag)

	DO_DESERIALIZE("transform", mTransform)

	std::list<Component *> tmpList;
	DO_DESERIALIZE_LIST("components", tmpList, [](const JSON &json)
	{
		Component *component = INSTANCE_BY_NAME(json["class"], Component);
		return component;
	})

	FOR_LIST(it, tmpList)
	{
		(*it)->init();
		addComponent(*it, (*it)->getClassId());
	}
}