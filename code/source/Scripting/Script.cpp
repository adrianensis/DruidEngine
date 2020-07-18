#include "Script.hpp"
#include "Log.hpp"

#include "GameObject.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Time2.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Script::Script() : Component() {
	mFirstStepDone = false;
}

// ---------------------------------------------------------------------------

Script::~Script() = default;

// ---------------------------------------------------------------------------

void Script::init() {
	mFirstStepDone = false;
}

// ---------------------------------------------------------------------------

void Script::firstStep() {

}

// ---------------------------------------------------------------------------

void Script::step() {

}

// ---------------------------------------------------------------------------

void Script::onEnterCollision(GameObject *otherGameObject) {

}

void Script::onCollision(GameObject *otherGameObject) {

}

void Script::onExitCollision(GameObject *otherGameObject) {

}
// ---------------------------------------------------------------------------

void Script::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
