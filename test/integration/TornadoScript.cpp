#include "TornadoScript.h"
#include "Log.h"

#include "GameObject.h"
#include "UI.h"
#include "Memory.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Collider.h"
#include "List.h"
#include "Vector4.h"
#include "Input.h"
#include "Mesh.h"

#include "Material.h"
#include "Texture.h"
#include "MaterialManager.h"
#include "Animation.h"
#include "Transform.h"
#include "Scene.h"
#include "Camera.h"
#include "Time.h"

namespace DE {

// ---------------------------------------------------------------------------

TornadoScript::TornadoScript() : Script(){

}

// ---------------------------------------------------------------------------

TornadoScript::~TornadoScript() = default;


void TornadoScript::init(){

}

// ---------------------------------------------------------------------------

void TornadoScript::firstStep(){
  getGameObject()->setTag("tornado");
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);

  mDurationTime = 5.0f;
  mDurationTimeCounter = 0;
}

// ---------------------------------------------------------------------------

void TornadoScript::step(){
  if(mDurationTimeCounter >= mDurationTime){
    ECHO("TORNADO DESTROY")
    getGameObject()->destroy();
  }
  mDurationTimeCounter += Time::getDeltaTimeSeconds();
}

// ---------------------------------------------------------------------------

void TornadoScript::onEnterCollision(GameObject* otherGameObject){

}

void TornadoScript::onCollision(GameObject* otherGameObject){

}

void TornadoScript::onExitCollision(GameObject* otherGameObject){

}

void TornadoScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
