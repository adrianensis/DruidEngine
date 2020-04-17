#include "PlayerScript.h"
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

PlayerScript::PlayerScript() : Script(){

}

// ---------------------------------------------------------------------------

PlayerScript::~PlayerScript() = default;


void PlayerScript::init(){

}

// ---------------------------------------------------------------------------

void PlayerScript::firstStep(){

  getGameObject()->setTag("player");
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
  mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
}

// ---------------------------------------------------------------------------

void PlayerScript::step(){

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

    Vector2 mouse = Input::getMousePosition();

    Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(mouse);

    Vector2 projectileOrigin = Vector2(getGameObject()->getTransform()->getWorldPosition().x + 50, getGameObject()->getTransform()->getWorldPosition().y);
    createProjectile(projectileOrigin.x, projectileOrigin.y, world.x, world.y);

  } else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    // mRenderer->setAnimation("run");
    // mRenderer->setInvertXAxis(true);
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    mRenderer->setAnimation("run");
    mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){
    mRigidBody->addLinear(Vector3(0,+1100,0));
  }else{
    mRenderer->setAnimation("idle");
    mRigidBody->addForce(Vector3(0,-4000,0));
  }
}

// ---------------------------------------------------------------------------

void PlayerScript::onEnterCollision(GameObject* otherGameObject){
  // std::string x = "onEnterCollision " + getGameObject()->getTag();
  // ECHO(x);
}

void PlayerScript::onCollision(GameObject* otherGameObject){

  // std::string x = "onCollision " + getGameObject()->getTag();
  // ECHO(x);
}

void PlayerScript::onExitCollision(GameObject* otherGameObject){
  // std::string x = "onExitCollision " + getGameObject()->getTag();
  // ECHO(x);
}


// ---------------------------------------------------------------------------

void PlayerScript::createProjectile(f32 x, f32 y, f32 clickX, f32 clickY){
  Vector2 size(150,150);

  Vector3 direction = Vector3(clickX, clickY,0).sub(Vector3(x,y,0));
  direction.nor();

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/projectile.png");

  GameObject* projectile = Memory::allocate<GameObject>();
  projectile->init();
  projectile->setTag("projectile");

  projectile->getTransform()->setLocalPosition(Vector3(x,y,0));
  projectile->getTransform()->setScale(Vector3(size.x,size.y,1));
  projectile->getTransform()->setRotation(Vector3(0,0,MathUtils::deg(std::atan(direction.y/direction.x))));

  Renderer* renderer = Memory::allocate<Renderer>();
  projectile->addComponent<Renderer>(renderer);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(2);

  renderer->addAnimation("fire", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/1.0f, 15));
  renderer->setAnimation("fire");

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  projectile->addComponent<RigidBody>(rigidBody);
  rigidBody->setLinear(direction*800);

  Collider* collider = Memory::allocate<Collider>();
  projectile->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);
  collider->setIsSolid(false);

  getGameObject()->getScene()->addGameObject(projectile);
}

// ---------------------------------------------------------------------------

void PlayerScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
