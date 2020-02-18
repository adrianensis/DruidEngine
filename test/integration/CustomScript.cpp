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
  Vector2 size(100/3.0f,100/3.0f);

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/mage.png");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(Shader::getDefaultShader());

  mTestObj = Memory::allocate<GameObject>();
  mTestObj->init();

  mTestObj->getTransform()->setLocalPosition(Vector3(-200,-200,0));
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
  collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(mTestObj);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestButton() {
  mTestButton = UI::getInstance()->createButton(Vector3(600,0,0), Vector2(400,100));

  mTestButton->setOnPressedCallback([&]() {
    mRenderer->setLineMode(true);
  });

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
  material->setShader(Shader::getDefaultShader());

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

  UIText* text = UI::getInstance()->createText(Vector2(world.x,world.y), Vector2(50,50), std::string("Hello stranger..."));

  getGameObject()->getScene()->addGameObject(text);
}

// ---------------------------------------------------------------------------

void CustomScript::createTestScene() {

  // Vector2 size(100.0f,100.0f);



  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/terrain.png");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(Shader::getDefaultShader());

  // FOR_RANGE(i, 0, 6){
  //   createTestTile((i*100.0f) - 500.0f, 0, material);
  // }
  //
  // FOR_RANGE(i, 5, 10){
  //   createTestTile((i*100.0f) - 500.0f, 100, material);
  // }
  //
  // FOR_RANGE(i, 0, 6){
  //   createTestTile((i*100.0f) - 500.0f, -400, material);
  // }
  //
  // FOR_RANGE(i, 5, 10){
  //   createTestTile((i*100.0f) - 500.0f, -300, material);
  // }

  createTestTile((100.0f) - 300.0f, -350, material);

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

void CustomScript::init(){
  mRigidBody = nullptr;
  mCollider = nullptr;
  mRenderer = nullptr;
  mTransform = nullptr;
  mTestObj = nullptr;
  mTestButton = nullptr;
  mTestCreated = false;
}

// ---------------------------------------------------------------------------

void CustomScript::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
  }

  if(!mTestCreated) {
    createTestObj();
    //createTestMap();
    createTestScene();
    mTestCreated = true;
    mRigidBody = mTestObj->getComponents<RigidBody>()->get(0);
    mCollider = mTestObj->getComponents<Collider>()->get(0);
    mRenderer = mTestObj->getComponents<Renderer>()->get(0);
    mTransform = mTestObj->getTransform();
  }

  f32 movement = 150.0f;//1.0f*Time::getDeltaTimeSeconds();

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

    // createFont();

    //Vector2 mouse(Input::getMousePosition());

    // VAR(f32, mouse.x);
    // VAR(f32, mouse.y);

    //Vector3 world = mCamera->screenToWorld(mouse);

    // VAR(f32, world.x);
    // VAR(f32, world.y);

    //File::readFile("resources/shaders/vertex.shader");

  }else if(Input::isKeyPressed(GLFW_KEY_KP_ADD)){
    mRenderer->setLineMode(true);
  }else if(Input::isKeyPressed(GLFW_KEY_KP_SUBTRACT)){
    mRenderer->setLineMode(false);
  }/*else if(Input::isKeyPressed(GLFW_KEY_SPACE)){
    mRigidBody->addForce(Vector3(0,1100.0f,0));
  }else if(Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)){
    mRigidBody->addForce(Vector3(0,-1000.0f,0));
  }*/

  else{
    mRigidBody->setLinear(Vector3(0,0,0));

  }

  if(running){
    mRenderer->setAnimation("run");
  }else{
    mRenderer->setAnimation("idle");
  }

  // mRigidBody->addForce(Vector3(0,-400.0f,0));
}

// ---------------------------------------------------------------------------

void CustomScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
