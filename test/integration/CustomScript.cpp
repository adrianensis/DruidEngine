#include "CustomScript.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Vector3.h"
#include "Time.h"
#include "Input.h"

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

  f32 movement = 1.0f*Time::getDeltaTimeSeconds();

  if(Input::isKeyPressed(GLFW_KEY_UP))
  {
    getGameObject()->getTransform()->translate(Vector3(0,movement,0));
  }
  else if(Input::isKeyPressed(GLFW_KEY_DOWN))
  {
    getGameObject()->getTransform()->translate(Vector3(0,-movement,0));
  }
  else if(Input::isKeyPressed(GLFW_KEY_LEFT))
  {
    getGameObject()->getTransform()->translate(Vector3(-movement,0,0));
  }
  else if(Input::isKeyPressed(GLFW_KEY_RIGHT))
  {
    getGameObject()->getTransform()->translate(Vector3(movement,0,0));
  }
}

// ---------------------------------------------------------------------------

void CustomScript::update() {

}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
