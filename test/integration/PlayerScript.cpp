#include "PlayerScript.h"
#include "Log.h"

#include "GameObject.h"

namespace DE {

// ---------------------------------------------------------------------------

PlayerScript::PlayerScript() : Script(){

}

// ---------------------------------------------------------------------------

PlayerScript::~PlayerScript() = default;


void PlayerScript::init(){

}

// ---------------------------------------------------------------------------

void PlayerScript::firstStep(){

}

// ---------------------------------------------------------------------------

void PlayerScript::step(){

}

// ---------------------------------------------------------------------------

void PlayerScript::onEnterCollision(GameObject* otherGameObject) {
  TRACE();
  otherGameObject->destroy();
}

void PlayerScript::onExitCollision(GameObject* otherGameObject) {
  TRACE();
}


// ---------------------------------------------------------------------------

void PlayerScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
