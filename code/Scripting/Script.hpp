#pragma once

#include "Scene/Component.hpp"

class GameObject;

CLASS(Script, Component)
{
	PRI(bool, FirstUpdateDone, GET)

public:
	virtual void init();
	virtual void firstUpdate();
	virtual void update();
	virtual void terminate();

	virtual void onEnterCollision(GameObject * otherGameObject);
	virtual void onCollision(GameObject * otherGameObject);
	virtual void onExitCollision(GameObject * otherGameObject);

	void firstUpdateDone()
	{
		mFirstUpdateDone = true;
	}
};