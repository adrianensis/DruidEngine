#include "Script.h"
#include "Log.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"
#include "Time.h"

namespace DE {

// ---------------------------------------------------------------------------

Script::Script() : Component(){
	mFirstStepDone = false;
}

// ---------------------------------------------------------------------------

Script::~Script() = default;

// ---------------------------------------------------------------------------

void Script::init(){

}

// ---------------------------------------------------------------------------

void Script::firstStep(){

}

// ---------------------------------------------------------------------------

void Script::step(){

}

// ---------------------------------------------------------------------------

void Script::onEnterCollision(GameObject* otherGameObject) {

}

void Script::onCollision(GameObject* otherGameObject) {

}

void Script::onExitCollision(GameObject* otherGameObject) {

}
// ---------------------------------------------------------------------------

void Script::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
