#include "Scene/GameObject.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"
#include "Config/ConfigObject.hpp"

GameObject::GameObject()
{
	mComponentsMap = nullptr;
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
	mShouldPersist = false;
}

GameObject::~GameObject()
{
	DELETE(mComponentsMap);
	DELETE(mTransform);
}

void GameObject::addComponent(Component *component, ClassId classId)
{
	if (!MAP_CONTAINS(*mComponentsMap, classId))
	{
		MAP_INSERT(*mComponentsMap, classId, NEW(std::list<Component *>));
	}

	mComponentsMap->at(classId)->push_back(component);

	component->setGameObject(this);
	component->init();
}

void GameObject::removeComponent(Component *component, ClassId classId)
{
	if (MAP_CONTAINS(*mComponentsMap, classId) && !component->getIsPendingToBeDestroyed() && !component->getIsDestroyed())
	{
		std::list<Component *> *list = mComponentsMap->at(classId);
		list->remove(component);
		component->destroy();
	}
}

void GameObject::init()
{
	// TRACE();

	mComponentsMap = NEW(std::map<ClassId, std::list<Component *> *>);

	mTransform = NEW(Transform);
	addComponent(mTransform);

	mTag = "";
}

const std::list<Component *> *GameObject::getComponents(ClassId classId) const
{
	std::list<Component *> *components = nullptr;

	if (MAP_CONTAINS(*mComponentsMap, classId))
	{
		components = mComponentsMap->at(classId);

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

	FOR_MAP(it, *mComponentsMap)
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

	FOR_MAP(it, *mComponentsMap)
	{
		auto list = it->second;

		FOR_LIST(itComponent, *list)
		{
			(*itComponent)->setGameObject(nullptr);
			(*itComponent)->destroy();
		}

		DELETE(list);
	}

	mComponentsMap->clear();
}

JSON GameObject::serialize() const
{
	return JSON();
}

void GameObject::deserialize(const JSON &json)
{
}

//void GameObject::serialize(ConfigObject* configMap, std::string& objectName) {
/*configMap->setString(objectName + ".class", getClassName());

	configMap->setBool(objectName + ".isStatic", getIsStatic());
	configMap->setBool(objectName + ".shouldPersist", getShouldPersist());

	configMap->setString(objectName + ".tag", getTag());

	Transform *t = getTransform();
	Vector3 worldPosition = t->getWorldPosition();
	Vector3 scale = t->getScale();

	configMap->setF32(objectName + ".worldPosition.x", worldPosition.x);
	configMap->setF32(objectName + ".worldPosition.y", worldPosition.y);
	configMap->setF32(objectName + ".size.width", scale.x);
	configMap->setF32(objectName + ".size.height", scale.y);

	// TODO : this property is not needed.
	Collider *collider = getFirstComponent<Collider>();
	configMap->setBool(objectName + ".hasCollider", collider ? true : false);

	FOR_LIST(it, mComponentsMap->getKeys()) {
		ClassId id = it.get();
		FOR_LIST(itComponent, mComponentsMap->get(id))
		{
			itComponent.get()->serialize(configMap, objectName);
		}
	}*/
//}

//void GameObject::deserialize(ConfigObject* configMap, std::string& objectName) {
/*setIsStatic(configMap->at(objectName + ".isStatic").get<bool>());
	setShouldPersist(configMap->at(objectName + ".shouldPersist").get<bool>());

	if(configMap->contains(objectName + ".tag")){
		setTag(configMap->getString(objectName + ".tag"));
	}

	Vector2 worldPosition(configMap->at(objectName + ".worldPosition.x").get<f32>(),
			configMap->at(objectName + ".worldPosition.y").get<f32>());

	Vector2 size(configMap->at(objectName + ".size.width").get<f32>(), configMap->at(objectName + ".size.height").get<f32>());
	
	getTransform()->setLocalPosition(Vector3(worldPosition.x, worldPosition.y, 0));
	getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = NEW(Renderer);
	addComponent<Renderer>(renderer);
	renderer->deserialize(configMap, objectName);

	if(configMap->at(objectName + ".hasCollider").get<bool>()) {
		RigidBody* rigidBody = NEW(RigidBody);
		addComponent<RigidBody>(rigidBody);

		Collider* collider = NEW(Collider);
		addComponent<Collider>(collider);
		collider->deserialize(configMap, objectName);
	}*/
//}