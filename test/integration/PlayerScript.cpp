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
#include "Time.h"
#include "ProjectileScript.h"

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

  mAttackAnimTimeCount = 0;
  mAttackAnimTime = 0.06f;
  mIsAttackPlaying = false;

  mBookColor.set(0,0,0,1);
  createBook(-90, 80, mBookColor);
  // createBook(-140, 20, Vector4(-0.5f,0,0.7f,1));
  // createBook(-160, 100, Vector4(-0.5f,0.35f,0.1f,1));

}

// ---------------------------------------------------------------------------

void PlayerScript::step(){

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

    Vector2 mouse = Input::getMousePosition();

    Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(mouse);

    Vector2 projectileOrigin = Vector2(getGameObject()->getTransform()->getWorldPosition().x + 50, getGameObject()->getTransform()->getWorldPosition().y);
    createProjectile(projectileOrigin.x, projectileOrigin.y, world.x, world.y);

    mRenderer->setAnimation("attack");
    mIsAttackPlaying = true;

  } else if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)){
    // mRenderer->setAnimation("idle");
  } else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    // mRenderer->setAnimation("run");
    // mRenderer->setInvertXAxis(true);
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    // mRenderer->setAnimation("run");
    // mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){
    mRigidBody->addLinear(Vector3(0,+1100,0));
  }else{

    if(mIsAttackPlaying){
      mAttackAnimTimeCount += Time::getDeltaTimeSeconds();

      if(mAttackAnimTimeCount >= mAttackAnimTime){
        mAttackAnimTimeCount = 0;
        mIsAttackPlaying = false;
      }
    }else{
      // mRenderer->setAnimation("idle");
      mRenderer->setAnimation("run");

    }
    mRigidBody->addForce(Vector3(0,-4000,0));
  }

  mBook->getComponents<Renderer>()->get(0)->setColor(mBookColor);

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
  Vector2 size(80,80);

  Vector3 direction = Vector3(clickX, clickY,0).sub(Vector3(x,y,0));
  direction.nor();

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/projectile.png");

  GameObject* projectile = Memory::allocate<GameObject>();
  projectile->init();

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

  renderer->setColor(mBookColor);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  projectile->addComponent<RigidBody>(rigidBody);
  rigidBody->setLinear(direction*800);

  Collider* collider = Memory::allocate<Collider>();
  projectile->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);
  collider->setIsSolid(false);

  Script* script = Memory::allocate<ProjectileScript>();
  projectile->addComponent<Script>(script);

  getGameObject()->getScene()->addGameObject(projectile);
}

// ---------------------------------------------------------------------------

void PlayerScript::createBook(f32 x, f32 y, const Vector4& color){
  Vector2 size(70,70);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/book.png");

  mBook = Memory::allocate<GameObject>();
  mBook->init();

  mBook->getTransform()->setLocalPosition(Vector3(x,y,0));
  mBook->getTransform()->setScale(Vector3(size.x,size.y,1));
  mBook->getTransform()->setParent(getGameObject()->getTransform());

  Renderer* renderer = Memory::allocate<Renderer>();
  mBook->addComponent<Renderer>(renderer);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(2);
  renderer->setColor(color);

  renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/1.0f, 6));
  renderer->setAnimation("idle");

  getGameObject()->getScene()->addGameObject(mBook);
}


// ---------------------------------------------------------------------------

void PlayerScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
