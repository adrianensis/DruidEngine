#pragma once

#include "Scene/Component.hpp"

class GameObject;

CLASS(Script, Component)
{
	PRI(FirstUpdateDone, GET, bool)

public:
	VIR void init();
	VIR void firstUpdate();
	VIR void update();
	VIR void terminate();

	VIR void onEnterCollision(GameObject * otherGameObject);
	VIR void onCollision(GameObject * otherGameObject);
	VIR void onExitCollision(GameObject * otherGameObject);

	void firstUpdateDone()
	{
		mFirstUpdateDone = true;
	}
};