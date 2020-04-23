#include "ProjectileScript.h"
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

ProjectileScript::ProjectileScript() : Script(){

}

// ---------------------------------------------------------------------------

ProjectileScript::~ProjectileScript() = default;


void ProjectileScript::init(){

}

// ---------------------------------------------------------------------------

void ProjectileScript::firstStep(){
  getGameObject()->setTag("projectile");
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);

  mExplosionTime = 0.3f;
  mExplosionTimeCounter = 0;
  mIsExploding = false;
}

// ---------------------------------------------------------------------------

void ProjectileScript::step(){
  // if(mRenderer->isOutOfCamera()){
  //   ECHO("PROJECTILE DESTROY")
  //   getGameObject()->destroy();
  // }

  if(mIsExploding){
    if(mExplosionTimeCounter >= mExplosionTime){
      ECHO("PROJECTILE DESTROY")
      getGameObject()->destroy();
    }
    mExplosionTimeCounter += Time::getDeltaTimeSeconds();
  }
}

// ---------------------------------------------------------------------------

void ProjectileScript::explode(){
  mIsExploding = true;
  mRenderer->setAnimation("explosion");
  getGameObject()->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,0,0));
}

// ---------------------------------------------------------------------------

void ProjectileScript::onEnterCollision(GameObject* otherGameObject){

}

void ProjectileScript::onCollision(GameObject* otherGameObject){

}

void ProjectileScript::onExitCollision(GameObject* otherGameObject){

}

void ProjectileScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
