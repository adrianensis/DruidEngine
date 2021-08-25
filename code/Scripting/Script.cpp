#include "Core/Time/TimeUtils.hpp"
#include "Scripting/Script.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Maths/Vector3.hpp"

void Script::init()
{
	mFirstUpdateDone = false;
}

void Script::firstUpdate()
{
}

void Script::update()
{
}

void Script::onEnterCollision(GameObject *otherGameObject)
{
}

void Script::onCollision(GameObject *otherGameObject)
{
}

void Script::onExitCollision(GameObject *otherGameObject)
{
}

void Script::terminate()
{
}