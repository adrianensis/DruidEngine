#include "CustomScript.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "Shader.h"
#include "Vector3.h"
#include "Time.h"
#include "Input.h"
#include "List.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Animation.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Scene.h"
#include "RenderEngine.h"

#include "File.h"

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::createTestObj() {
  Vector2 size(100,100);

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/mage.bmp");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(mRenderer->getMaterial()->getShader());

  mTestObj = Memory::allocate<GameObject>();
  mTestObj->init();

  mTestObj->getTransform()->setLocalPosition(Vector3(400,-400,0));
  mTestObj->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mTestObj->addComponent<Renderer>(renderer);

  renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->setAnimation("idle");

  renderer->setLineMode(true);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mTestObj->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mTestObj->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(mTestObj);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestMap() {

  GameObject* tile;

  FOR_RANGE(i, 0, 16){
    FOR_RANGE(j, 0, 16){

      Vector2 size(200,200);

      Texture* texture = Memory::allocate<Texture>();
      texture->init("resources/terrain.png");

      Material* material = Memory::allocate<Material>();
      material->init();
      material->setTexture(texture);
      material->setShader(mRenderer->getMaterial()->getShader());

      tile = Memory::allocate<GameObject>();
      tile->init();

      tile->getTransform()->setLocalPosition(Vector3(i * size.x, j * size.y,0));
      tile->getTransform()->setScale(Vector3(size.x,size.y,1));

      Renderer* renderer = Memory::allocate<Renderer>();
      tile->addComponent<Renderer>(renderer);

      renderer->setColor(Vector4(0,0,0,0.7f));

      renderer->setMesh(Mesh::getRectangle());
      renderer->setMaterial(material);

      renderer->setRegion(i/16.0f, j/16.0f, 1.0f/16.0f, 1.0f/16.0f);

      // RigidBody* rigidBody = Memory::allocate<RigidBody>();
      // tile->addComponent<RigidBody>(rigidBody);
      //
      // Collider* collider = Memory::allocate<Collider>();
      // tile->addComponent<Collider>(collider);
      // collider->setSize(size.x,size.y);

      getGameObject()->getScene()->addGameObject(tile);
    }
  }

}

// ---------------------------------------------------------------------------

void CustomScript::init(){
  mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
  mTransform = getGameObject()->getTransform();
  mTestObj = nullptr;
}

// ---------------------------------------------------------------------------

void CustomScript::step(){

  f32 movement = 500.0f;//1.0f*Time::getDeltaTimeSeconds();

  bool running = false;

  if(Input::isKeyPressed(GLFW_KEY_UP))
  {
    // mTransform->translate(Vector3(0,movement,0));
    // mRigidBody->addForce(Vector3(0,movement,0));
    mRigidBody->setLinear(Vector3(0,movement,0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
    // mTransform->translate(Vector3(0,-movement,0));
    // mRigidBody->addForce(Vector3(0,-movement,0));
    mRigidBody->setLinear(Vector3(0,-movement,0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    // mTransform->translate(Vector3(-movement,0,0));
    // mRigidBody->addForce(Vector3(-movement,0,0));
    mRigidBody->setLinear(Vector3(-movement,0,0));
    running = true;
    mRenderer->setInvertXAxis(true);
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    // mTransform->translate(Vector3(movement,0,0));
    // mRigidBody->addForce(Vector3(movement,0,0));
    mRigidBody->setLinear(Vector3(movement,0,0));
    running = true;
    mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressed(GLFW_KEY_ENTER)){
    if(!mTestObj) {
      createTestMap();
      createTestObj();

      RenderEngine::getInstance()->drawLine(Vector3(0,0,0), Vector3(100,100,0));
    }

    // Vector2 mouse(Input::getMousePosition());
    // VAR(f32, mouse.x);
    // VAR(f32, mouse.y);

    File::readFile("resources/shaders/vertex.shader");
  }else if(Input::isKeyPressed(GLFW_KEY_KP_ADD)){
    mRenderer->setLineMode(true);
  }else if(Input::isKeyPressed(GLFW_KEY_KP_SUBTRACT)){
    mRenderer->setLineMode(false);
  }else{
    mRigidBody->setLinear(Vector3(0,0,0));
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
