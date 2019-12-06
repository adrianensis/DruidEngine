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

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Animation.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Scene.h"
#include "RenderEngine.h"
#include "RenderContext.h"

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

  //renderer->setLineMode(true);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mTestObj->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mTestObj->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(mTestObj);
}


// ---------------------------------------------------------------------------

void CustomScript::createTestButton() {
  Vector2 size(400,100);

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/button.bmp");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(mRenderer->getMaterial()->getShader());

  mTestButton = Memory::allocate<GameObject>();
  mTestButton->init();

  mTestButton->getTransform()->setLocalPosition(Vector3(400,0,0));
  mTestButton->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mTestButton->addComponent<Renderer>(renderer);

  renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mTestButton->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mTestButton->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(mTestButton);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestMap() {

  Vector2 size(100.0f,100.0f);

  GameObject* map = Memory::allocate<GameObject>();
  map->init();

  map->getTransform()->setLocalPosition(Vector3(0,0,0));
  map->getTransform()->setScale(Vector3(size.x, size.y,1));

  f32 tilesCount = 16;
  f32 tileTextureSize = 1.0f/tilesCount;

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/terrain.png");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(mRenderer->getMaterial()->getShader());

  FOR_RANGE(i, 0, tilesCount){
    FOR_RANGE(j, 0, tilesCount){

      Renderer* renderer = Memory::allocate<Renderer>();
      map->addComponent<Renderer>(renderer);

      //renderer->setColor(Vector4(0,0,0,0.7f));
      renderer->setPositionOffset(Vector3((i - tilesCount/2.0f) * size.x, (j - tilesCount/2.0f) * size.y,0));

      renderer->setMesh(Mesh::getRectangle());
      renderer->setMaterial(material);

      renderer->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);
    }
  }

  getGameObject()->getScene()->addGameObject(map);
}

// ---------------------------------------------------------------------------

void CustomScript::init(){
  mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
  mCollider = getGameObject()->getComponents<Collider>()->get(0);
  mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
  mTransform = getGameObject()->getTransform();
  mTestObj = nullptr;
  mTestButton = nullptr;
  mTestCreated = false;
}

// ---------------------------------------------------------------------------

void CustomScript::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
  }

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
  // }else if(Input::isKeyPressed(GLFW_KEY_ENTER)){
  }else if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
    if(!mTestCreated) {
      createTestButton();
      createTestObj();

      mTestCreated = true;

      RenderEngine::getInstance()->drawLine(Vector3(0,0,0), Vector3(100,100,0));
    }

    Vector2 mouse(Input::getMousePosition());

    // VAR(f32, mouse.x);
    // VAR(f32, mouse.y);

    Vector3 world = mCamera->screenToWorld(mouse);

    // VAR(f32, world.x);
    // VAR(f32, world.y);

    if(mTestButton->getComponents<Collider>()->get(0)->testPoint(world) != ColliderStatus::STATUS_NONE){
      mRenderer->setLineMode(true);
      mTestObj->getComponents<Renderer>()->get(0)->setLineMode(true);
      mTestButton->getComponents<Renderer>()->get(0)->setColor(Vector4(0,1,0,1));
    }

    //File::readFile("resources/shaders/vertex.shader");

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

  counter++;
}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
