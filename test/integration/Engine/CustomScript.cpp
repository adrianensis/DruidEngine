#include "CustomScript.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"
#include "Time.h"

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::init(){

}

// ---------------------------------------------------------------------------

void CustomScript::step(){
  ECHO("CUSTOM SCRIPT STEP");
}

// ---------------------------------------------------------------------------

void CustomScript::update() {

}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
