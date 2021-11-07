#pragma once

#include "Core/Core.hpp"

class GameObject;

class Component: public ObjectBase
{
    GENERATE_METADATA(Component)
	PRI bool mIsActive = false;
	PRI bool mIsStatic = false;

	PRI GameObject* mGameObject; GET_SET(GameObject)
	PRI bool mAlreadyAddedToEngine = false; GET_SET(AlreadyAddedToEngine)
	PRI bool mIsPendingToBeDestroyed = false; GET(IsPendingToBeDestroyed)
	PRI bool mIsDestroyed = false; GET(IsDestroyed)

PUB
	Component();

	virtual void init() = 0;

	virtual void onComponentAdded() { };

	bool isStatic();

	bool isActive() const
	{
		return (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : mIsActive;
	};

	void setIsActive(bool isActive)
	{
		mIsActive = (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : isActive;
	};

	void finallyDestroy()
	{
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	virtual void onDestroy(){};

	virtual void serialize(JSON &json) const override;
	virtual void deserialize(const JSON &json) override;
};