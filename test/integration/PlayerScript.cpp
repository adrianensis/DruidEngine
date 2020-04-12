#include "PlayerScript.h"
#include "Log.h"

#include "GameObject.h"
#include "UI.h"
#include "Memory.h"
#include "Renderer.h"
#include "List.h"
#include "Vector4.h"

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

  getGameObject()->setTag("player");
  // text = nullptr;
}

// ---------------------------------------------------------------------------

void PlayerScript::step(){

}

// ---------------------------------------------------------------------------

void PlayerScript::onEnterCollision(GameObject* otherGameObject) {
  // TRACE();
  // if(textEnter){
  //   textEnter->destroy();
  // }
  //
  // if(textExit){
  //   textExit->destroy();
  // }
  //
  // if(textCollision){
  //   textCollision->destroy();
  // }
  //
  //
  // textEnter = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(-500,200), Vector2(50,50), std::string("ON ENTER COLLISION"), 5);

  std::string x = "onEnterCollision " + getGameObject()->getTag();
  ECHO(x);

  // otherGameObject->getComponents<Renderer>()->get(0)->setColor(Vector4(0,1,0,1));
}

void PlayerScript::onCollision(GameObject* otherGameObject) {
  // TRACE();
  // if(textEnter){
  //   textEnter->destroy();
  // }
  //
  // if(textExit){
  //   textExit->destroy();
  // }
  //
  // if(textCollision){
  //   textCollision->destroy();
  // }
  //
  // textCollision = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(-500, 300), Vector2(50,50), std::string("ON COLLISION"), 5);

  std::string x = "onCollision " + getGameObject()->getTag();
  // ECHO(x);
}

void PlayerScript::onExitCollision(GameObject* otherGameObject) {
  // TRACE();


  // if(textEnter){
  //   textEnter->destroy();
  // }
  //
  // if(textExit){
  //   textExit->destroy();
  // }
  //
  // if(textCollision){
  //   textCollision->destroy();
  // }
  //
  //
  // textExit = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(-500, 400), Vector2(50,50), std::string("ON EXIT COLLISION"), 5);

  std::string x = "onExitCollision " + getGameObject()->getTag();
  ECHO(x);
  // otherGameObject->getComponents<Renderer>()->get(0)->setColor(Vector4(1,0,0,1));

}


// ---------------------------------------------------------------------------

void PlayerScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
