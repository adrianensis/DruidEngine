#include "CustomScript.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Vector3.h"
#include "Time.h"
#include "Input.h"
#include "List.h"

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::init(){
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
  mTransform = getGameObject()->getTransform();
}

// ---------------------------------------------------------------------------

void CustomScript::step(){

  f32 movement = 1.0f*Time::getDeltaTimeSeconds();

  bool running = false;

  if(Input::isKeyPressed(GLFW_KEY_UP))
  {
    mTransform->translate(Vector3(0,movement,0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }
  else if(Input::isKeyPressed(GLFW_KEY_DOWN))
  {
    mTransform->translate(Vector3(0,-movement,0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }
  else if(Input::isKeyPressed(GLFW_KEY_LEFT))
  {
    mTransform->translate(Vector3(-movement,0,0));
    running = true;
    mRenderer->setInvertXAxis(true);
  }
  else if(Input::isKeyPressed(GLFW_KEY_RIGHT))
  {
    mTransform->translate(Vector3(movement,0,0));
    running = true;
    mRenderer->setInvertXAxis(false);
  }


  if(running){
    mRenderer->setAnimation("run");
  }else{
    mRenderer->setAnimation("idle");
  }
}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
