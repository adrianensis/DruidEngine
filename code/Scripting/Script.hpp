#pragma once

#include "Scene/Component.hpp"

class GameObject;

CLASS(Script, Component)
{
	PRI(FirstStepDone, GET, bool)

public:
	virtual void init();
	virtual void firstStep();
	virtual void step();
	virtual void terminate();

	virtual void onEnterCollision(GameObject * otherGameObject);
	virtual void onCollision(GameObject * otherGameObject);
	virtual void onExitCollision(GameObject * otherGameObject);

	void firstStepDone()
	{
		mFirstStepDone = true;
	}
};