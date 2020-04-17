#include "EnemyScript.h"
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

namespace DE {

// ---------------------------------------------------------------------------

EnemyScript::EnemyScript() : Script(){

}

// ---------------------------------------------------------------------------

EnemyScript::~EnemyScript() = default;


void EnemyScript::init(){

}

// ---------------------------------------------------------------------------

void EnemyScript::firstStep(){

  getGameObject()->setTag("enemy");
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
  mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
}

// ---------------------------------------------------------------------------

void EnemyScript::step(){

}

// ---------------------------------------------------------------------------

void EnemyScript::onEnterCollision(GameObject* otherGameObject){
  std::string x = "onEnterCollision " + getGameObject()->getTag();
  ECHO(x);

  if(otherGameObject->getTag() == "projectile"){
    getGameObject()->destroy();
  }
}

void EnemyScript::onCollision(GameObject* otherGameObject){

  // std::string x = "onCollision " + getGameObject()->getTag();
  // // ECHO(x);
}

void EnemyScript::onExitCollision(GameObject* otherGameObject){
  // std::string x = "onExitCollision " + getGameObject()->getTag();
  // ECHO(x);
}


// ---------------------------------------------------------------------------

void EnemyScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
