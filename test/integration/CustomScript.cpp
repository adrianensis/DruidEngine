#include "CustomScript.h"
#include "Log.h"

#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Time.h"
#include "Input.h"
#include "List.h"
#include "HashMap.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "MaterialManager.h"
#include "Animation.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Scene.h"
#include "RenderEngine.h"
#include "RenderContext.h"

#include "File.h"
#include "UI.h"
#include "UIButton.h"
#include "UIText.h"
#include "PhysicsEngine.h"

#include "ScenesManager.h"

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::createPlayer(f32 x, f32 y){
  Vector2 size(100*1.5f,100*1.5f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

  mPlayer = Memory::allocate<GameObject>();
  mPlayer->init();

  mPlayer->getTransform()->setLocalPosition(Vector3(x,y,0));
  mPlayer->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mPlayer->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(1);

  Vector2 atlasSize = Vector2(8,16);

  renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,14.0f/16.0f), 1.0f/8.0f, 1.0f/16.0f, 10));
  renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,15.0f/16.0f), 1.0f/8.0f, 1.0f/16.0f, 10));
  renderer->setAnimation("idle");

  // renderer->setRegion(0.0f, 14.0f/atlasSize.y, 6.0f/atlasSize.x, 2.0f/atlasSize.y);

  //renderer->setLineMode(true);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mPlayer->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mPlayer->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  getGameObject()->getScene()->addGameObject(mPlayer);
}
// ---------------------------------------------------------------------------

void CustomScript::createBox(f32 x, f32 y){
  Vector2 size(100,100);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

  mBox = Memory::allocate<GameObject>();
  mBox->init();
  mBox->setIsStatic(true);

  mBox->getTransform()->setLocalPosition(Vector3(x,y,0));
  mBox->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mBox->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(1);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mBox->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mBox->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  getGameObject()->getScene()->addGameObject(mBox);
}

// ---------------------------------------------------------------------------

void CustomScript::init(){

}

// ---------------------------------------------------------------------------

void CustomScript::firstStep(){

}

// ---------------------------------------------------------------------------

void CustomScript::step(){
  if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){
    ScenesManager* sceneManager = ScenesManager::getInstance();
    sceneManager->setScene(1);
  } else if(Input::isKeyPressedOnce(GLFW_KEY_P)){

    FOR_RANGE(i, -3, 3){
      FOR_RANGE(j, -3, 3){
        createPlayer(((f32)i)*300 + 5,((f32)j)*300 + 5);
      }
    }

    createPlayer(125,125);
  } else if(Input::isKeyPressedOnce(GLFW_KEY_B)){

    createBox(340,250);
  }

  if(mPlayer){
    f32 movement = 150;
    if(Input::isKeyPressed(GLFW_KEY_UP)){
      mPlayer->getComponents<Renderer>()->get(0)->setAnimation("run");
      // mPlayer->getTransform()->translate(Vector3(0,movement,0));
      mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,movement,0));
      getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(0,1,0));
    }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
      mPlayer->getComponents<Renderer>()->get(0)->setAnimation("run");
      // mPlayer->getTransform()->translate(Vector3(0,-movement,0));
      mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,-movement,0));
      getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(0,-1,0));
    }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
      mPlayer->getComponents<Renderer>()->get(0)->setAnimation("run");
      mPlayer->getComponents<Renderer>()->get(0)->setInvertXAxis(true);
      // mPlayer->getTransform()->translate(Vector3(-movement,0,0));
      mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(-movement,0,0));
      getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(-1,0,0));
    }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
      mPlayer->getComponents<Renderer>()->get(0)->setAnimation("run");
      mPlayer->getComponents<Renderer>()->get(0)->setInvertXAxis(false);
      // mPlayer->getTransform()->translate(Vector3(movement,0,0));
      mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(movement,0,0));
      getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(1,0,0));
    }else{
      mPlayer->getComponents<Renderer>()->get(0)->setAnimation("idle");
      mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,0,0));

    }
  }
}

// ---------------------------------------------------------------------------

void CustomScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
