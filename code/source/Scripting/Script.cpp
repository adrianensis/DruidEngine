#include "Script.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"

namespace DE {

// ---------------------------------------------------------------------------

Script::Script() : Component(){

}

// ---------------------------------------------------------------------------

Script::~Script() = default;

// ---------------------------------------------------------------------------

void Script::init(){
  x = 0;
}

// ---------------------------------------------------------------------------

void Script::step(){
  //ECHO("CUSTOM SCRIPT STEP")

  getGameObject()->getTransform()->translate(Vector3(1.0f,0,0));
  //getGameObject()->getTransform()->rotate(Vector3(0,0.5f,0.0f));
  //getGameObject()->getTransform()->lookAt(Vector3(0,0,0));

  x += 1.0f;
}

// ---------------------------------------------------------------------------

void Script::update() {
}

// ---------------------------------------------------------------------------

void Script::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
