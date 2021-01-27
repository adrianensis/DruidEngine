#include "Core/TimeUtils.hpp"
#include "Scripting/Script.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

Script::Script() : Component() {
	mFirstStepDone = false;
}

Script::~Script() = default;

void Script::init() {
	mFirstStepDone = false;
}

void Script::firstStep() {

}

void Script::step() {

}

void Script::onEnterCollision(GameObject *otherGameObject) {

}

void Script::onCollision(GameObject *otherGameObject) {

}

void Script::onExitCollision(GameObject *otherGameObject) {

}

void Script::terminate() {

}

}
