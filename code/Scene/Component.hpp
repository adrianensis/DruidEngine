#pragma once

#include "Core/Core.hpp"

class GameObject;

CLASS(Component, ObjectBase)
{
	PRI(IsActive, NONE, bool)
	PRI(IsStatic, NONE, bool)

	PRI(GameObject, GET_SET, GameObject *)
	PRI(AlreadyAddedToEngine, GET_SET, bool)
	PRI(IsPendingToBeDestroyed, GET, bool)
	PRI(IsDestroyed, GET, bool)

public:
	Component();

	VIR void init() = 0;

	VIR void onComponentAdded() { };

	bool isStatic();

	bool isActive() CNS
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

	VIR void onDestroy(){};

	SERIALIZE();
	DESERIALIZE();
};