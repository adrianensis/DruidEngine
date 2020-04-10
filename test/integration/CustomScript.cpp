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

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::createTestObj() {
  Vector2 size(100*1.5f,100*1.5f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage.png");

  mTestObj = Memory::allocate<GameObject>();
  mTestObj->init();

  mTestObj->getTransform()->setLocalPosition(Vector3(-300,-100,0));
  mTestObj->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mTestObj->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->setAnimation("idle");

  //renderer->setLineMode(true);
  renderer->setLayer(1);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mTestObj->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mTestObj->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  Script* script = Memory::allocate<PlayerScript>();
  mTestObj->addComponent<Script>(script);

  // mTestMinion = Memory::allocate<GameObject>();
  // mTestMinion->init();
  //
  // mTestMinion->getTransform()->setLocalPosition(Vector3(-200,-30,0));
  // mTestMinion->getTransform()->setScale(Vector3(size.x/2.0f,size.y/2.0f,1));
  //
  // Renderer* renderer2 = Memory::allocate<Renderer>();
  // mTestMinion->addComponent<Renderer>(renderer2);
  //
  // renderer2->setMesh(Mesh::getRectangle());
  // renderer2->setMaterial(material);
  //
  // renderer2->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
  // renderer2->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
  // renderer2->setAnimation("idle");
  //
  // mTestMinion->getTransform()->setParent(mTestObj->getTransform());

  // getGameObject()->getScene()->addGameObject(mTestMinion);
  getGameObject()->getScene()->addGameObject(mTestObj);

  getGameObject()->getScene()->getCameraGameObject()->getTransform()->setParent(mTestObj->getTransform());
}

// ---------------------------------------------------------------------------

void CustomScript::createTestButton() {
  mTestButton = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3(600,0,0), Vector2(400,100), 5);

  mTestButton->setOnPressedCallback([&]() {
    mRenderer->setLineMode(true);
  });
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

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/terrain.png");

  FOR_RANGE(i, 0, tilesCount){
    FOR_RANGE(j, 0, tilesCount){

      Renderer* renderer = Memory::allocate<Renderer>();
      map->addComponent<Renderer>(renderer);

      renderer->setPositionOffset(Vector3((i - tilesCount/2.0f) * size.x, (j - tilesCount/2.0f) * size.y,0));

      renderer->setMesh(Mesh::getRectangle());
      renderer->setMaterial(material);

      renderer->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);
    }
  }

  map->setIsStatic(true);

  getGameObject()->getScene()->addGameObject(map);
}

// ---------------------------------------------------------------------------

void CustomScript::createFont() {

  Vector2 mouse(Input::getMousePosition());
  Vector3 world = mCamera->screenToWorld(mouse);

  UIText* text = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(world.x,world.y), Vector2(50,50), std::string("Hello stranger..."), 5);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestScene() {

  // Vector2 size(100.0f,100.0f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/terrain.png");

  FOR_RANGE(i, 0, 6){
    createTestTile((i*100.0f) - 500.0f, 100, material);
  }

  FOR_RANGE(i, 5, 10){
    createTestTile((i*100.0f) - 500.0f, 200, material);
  }

  FOR_RANGE(i, 0, 6){
    createTestTile((i*100.0f) - 500.0f, -300, material);
  }

  FOR_RANGE(i, 5, 10){
    createTestTile((i*100.0f) - 500.0f, -200, material);
  }

  //createTestTile((100.0f) - 300.0f, -350, material);

}

// ---------------------------------------------------------------------------

void CustomScript::createTestTile(float x, float y, Material* material) {
  Vector2 size(100.0f,100.0f);

  f32 tilesCount = 16;
  f32 tileTextureSize = 1.0f/tilesCount;

  GameObject* tile = Memory::allocate<GameObject>();
  tile->init();

  tile->getTransform()->setLocalPosition(Vector3(x, y,0));
  tile->getTransform()->setScale(Vector3(size.x, size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  tile->addComponent<Renderer>(renderer);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setRegion(4/tilesCount, 1/tilesCount, tileTextureSize, tileTextureSize);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  tile->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  tile->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  tile->setIsStatic(true);

  getGameObject()->getScene()->addGameObject(tile);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestBackground(float x, float y){
  Vector2 size(2200.0f,750.0f);

  GameObject* mountain = Memory::allocate<GameObject>();
  mountain->init();

  mountain->getTransform()->setLocalPosition(Vector3(0,100,0));
  mountain->getTransform()->setScale(Vector3(size.x, size.y,1));

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/mountain.png");

  Renderer* mRendererMountain3 = Memory::allocate<Renderer>();
  mountain->addComponent<Renderer>(mRendererMountain3);

  mRendererMountain3->setPositionOffset(Vector3(0,300,0));

  mRendererMountain3->setMesh(Mesh::getRectangle());
  mRendererMountain3->setMaterial(material);
  mRendererMountain3->setColor(Vector4(-0.25f,-0.25f,-0.3f,0.3f));

  // renderer 1
  mRendererMountain = Memory::allocate<Renderer>();
  mountain->addComponent<Renderer>(mRendererMountain);

  mRendererMountain->setPositionOffset(Vector3(-200,200,0));

  mRendererMountain->setMesh(Mesh::getRectangle());
  mRendererMountain->setMaterial(material);
  mRendererMountain->setColor(Vector4(-0.2f,-0.3f,-0.3f,1.0f));

  // renderer 2
  mRendererMountain2 = Memory::allocate<Renderer>();
  mountain->addComponent<Renderer>(mRendererMountain2);

  mRendererMountain2->setPositionOffset(Vector3(150,0,0));

  mRendererMountain2->setMesh(Mesh::getRectangle());
  mRendererMountain2->setMaterial(material);
  mRendererMountain2->setColor(Vector4(-0.1f,-0.1f,-0.1f,1.0f));


  //mountain->setIsStatic(true);

  getGameObject()->getScene()->addGameObject(mountain);

  Vector2 size2(1700.0f,800.0f);

  GameObject* forest = Memory::allocate<GameObject>();
  forest->init();

  forest->getTransform()->setLocalPosition(Vector3(0,-270,0));
  forest->getTransform()->setScale(Vector3(size2.x, size2.y,1));

  Material* material2 = MaterialManager::getInstance()->loadMaterial("resources/forest.png");

  // renderer 1
  mRendererForest = Memory::allocate<Renderer>();
  forest->addComponent<Renderer>(mRendererForest);

  mRendererForest->setPositionOffset(Vector3(100,50,0));

  mRendererForest->setMesh(Mesh::getRectangle());
  mRendererForest->setMaterial(material2);
  mRendererForest->setColor(Vector4(-0.15f,-0.15f,-0.00f,1.0f));

  // renderer 2
  mRendererForest2 = Memory::allocate<Renderer>();
  forest->addComponent<Renderer>(mRendererForest2);

  mRendererForest2->setPositionOffset(Vector3(0,0,0));

  mRendererForest2->setMesh(Mesh::getRectangle());
  mRendererForest2->setMaterial(material2);
  mRendererForest2->setColor(Vector4(-0.05f,-0.05f,-0.05f,1.0f));


  //mountain->setIsStatic(true);

  getGameObject()->getScene()->addGameObject(forest);
}

// ---------------------------------------------------------------------------

void CustomScript::init(){
  mRigidBody = nullptr;
  mCollider = nullptr;
  mRenderer = nullptr;
  mTransform = nullptr;
  mTestObj = nullptr;
  mTestButton = nullptr;
  mTestCreated = false;

  mRendererMountain = nullptr;
  mRendererMountain2 = nullptr;
  mRendererForest = nullptr;
  mRendererForest2 = nullptr;
}

// ---------------------------------------------------------------------------

void CustomScript::firstStep(){
    if(! mCamera){
      mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    }

    if(!mTestCreated) {
      // createTestBackground(0,0);
      createTestObj();
      // createTestMap();
      createTestScene();
      mTestCreated = true;
      mRigidBody = mTestObj->getComponents<RigidBody>()->get(0);
      mCollider = mTestObj->getComponents<Collider>()->get(0);
      mRenderer = mTestObj->getComponents<Renderer>()->get(0);
      // mRendererMinion = mTestMinion->getComponents<Renderer>()->get(0);
      mTransform = mTestObj->getTransform();
    }
}

// ---------------------------------------------------------------------------

void CustomScript::step(){
  f32 movement = 650;

  bool running = false;

  if(Input::isKeyPressed(GLFW_KEY_UP))
  {
    // mTransform->translate(Vector3(0,movement,0));
    mRigidBody->setLinear(Vector3(0,movement,0));
    // getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(0,movement*Time::getDeltaTimeSeconds(),0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
    // mTransform->translate(Vector3(0,-movement,0));
    mRigidBody->setLinear(Vector3(0,-movement,0));
    // getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(0,-movement*Time::getDeltaTimeSeconds(),0));
    running = true;
    // mRenderer->setInvertXAxis(false);
  }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    // mTransform->translate(Vector3(-movement,0,0));
    mRigidBody->setLinear(Vector3(-movement,0,0));
    // getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(-movement*Time::getDeltaTimeSeconds(),0,0));
    // mRendererMountain2->setPositionOffset(mRendererMountain2->getPositionOffset() + Vector3((movement/(6.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererMountain->setPositionOffset(mRendererMountain->getPositionOffset() + Vector3((movement/(10.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererForest2->setPositionOffset(mRendererForest2->getPositionOffset() + Vector3((movement/(2.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererForest->setPositionOffset(mRendererForest->getPositionOffset() + Vector3((movement/(3.0f))*Time::getDeltaTimeSeconds(), 0,0));
    running = true;
    mRenderer->setInvertXAxis(true);
    // mRendererMinion->setInvertXAxis(true);
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    // mTransform->translate(Vector3(movement,0,0));
    mRigidBody->setLinear(Vector3(movement,0,0));
    // getGameObject()->getScene()->getCameraGameObject()->getTransform()->translate(Vector3(movement*Time::getDeltaTimeSeconds(),0,0));
    // mRigidBody->setLinear(Vector3(movement,0,0));
    // mRendererMountain2->setPositionOffset(mRendererMountain2->getPositionOffset() + Vector3((-movement/(6.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererMountain->setPositionOffset(mRendererMountain->getPositionOffset() + Vector3((-movement/(10.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererForest2->setPositionOffset(mRendererForest2->getPositionOffset() + Vector3((-movement/(2.0f))*Time::getDeltaTimeSeconds(), 0,0));
    // mRendererForest->setPositionOffset(mRendererForest->getPositionOffset() + Vector3((-movement/(3.0f))*Time::getDeltaTimeSeconds(), 0,0));
    running = true;
    mRenderer->setInvertXAxis(false);
    // mRendererMinion->setInvertXAxis(false);
  // }else if(Input::isKeyPressedOnce(GLFW_KEY_ENTER)){
  }else if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

    // createFont();

    //Vector2 mouse(Input::getMousePosition());

    // VAR(f32, mouse.x);
    // VAR(f32, mouse.y);

    //Vector3 world = mCamera->screenToWorld(mouse);

    // VAR(f32, world.x);
    // VAR(f32, world.y);

  }else if(Input::isKeyPressedOnce(GLFW_KEY_KP_ADD)){
    mRenderer->setLineMode(true);
  }else if(Input::isKeyPressedOnce(GLFW_KEY_KP_SUBTRACT)){
    mRenderer->setLineMode(false);
  }/*else if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){
    mRigidBody->addForce(Vector3(0,1100.0f,0));
  }else if(Input::isKeyPressedOnce(GLFW_KEY_LEFT_SHIFT)){
    mRigidBody->addForce(Vector3(0,-1000.0f,0));
  }*/

  else{
    mRigidBody->setLinear(Vector3(0,0,0));

  }

  if(running){
    mRenderer->setAnimation("run");
    // mRendererMinion->setAnimation("run");
  }else{
    mRenderer->setAnimation("idle");
    // mRendererMinion->setAnimation("idle");
  }

  // mRigidBody->addForce(Vector3(0,-400.0f,0));
}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
