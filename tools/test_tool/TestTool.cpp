#include "TestTool.h"
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

TestTool::TestTool() : Script(){

}

// ---------------------------------------------------------------------------

TestTool::~TestTool() = default;

// ---------------------------------------------------------------------------

void TestTool::createTestObj() {
  Vector2 size(100,100);

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/mage.bmp");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(Shader::getDefaultShader());

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

void TestTool::createTestButton() {
  mTestButton = UI::getInstance()->createButton(Vector3(600,0,0), Vector2(400,100));

  mTestButton->setOnPressedCallback([&]() {

  });

  getGameObject()->getScene()->addGameObject(mTestButton);
}

// ---------------------------------------------------------------------------

void TestTool::createTile() {

  Vector2 size(100.0f,100.0f);

  GameObject* tile = Memory::allocate<GameObject>();
  tile->init();

  tile->getTransform()->setLocalPosition(Vector3(0,0,0));
  tile->getTransform()->setScale(Vector3(size.x, size.y,1));

  f32 tilesCount = 16;
  f32 tileTextureSize = 1.0f/tilesCount;

  if(!mTexture){
    mTexture = Memory::allocate<Texture>();
    mTexture->init("resources/terrain.png");

    mMaterial = Memory::allocate<Material>();
    mMaterial->init();
    mMaterial->setTexture(mTexture);
    mMaterial->setShader(Shader::getDefaultShader());
  }

  Renderer* renderer = Memory::allocate<Renderer>();
  tile->addComponent<Renderer>(renderer);

  Vector2 mouse(Input::getMousePosition());

  VAR(f32, mouse.x);
  VAR(f32, mouse.y);

  Vector3 world = mCamera->screenToWorld(mouse);

  VAR(f32, world.x);
  VAR(f32, world.y);

  renderer->setPositionOffset(world/*Vector3((i - tilesCount/2.0f) * size.x, (j - tilesCount/2.0f) * size.y,0)*/);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(mMaterial);

  renderer->setRegion(mTileIndex/tilesCount, mTileIndex/tilesCount, tileTextureSize, tileTextureSize);

  getGameObject()->getScene()->addGameObject(tile);
}

void TestTool::createFont() {

  Vector2 mouse(Input::getMousePosition());
  Vector3 world = mCamera->screenToWorld(mouse);

  UIText* text = UI::getInstance()->createText(Vector2(world.x,world.y), Vector2(50,50), std::string("Hello stranger..."));

  getGameObject()->getScene()->addGameObject(text);
}

// ---------------------------------------------------------------------------

void TestTool::init(){
  mTransform = getGameObject()->getTransform();
  mTestObj = nullptr;
  mTestButton = nullptr;
  mTestCreated = false;
  mTexture = nullptr;
  mMaterial = nullptr;

  mCamera = nullptr;

  mTileIndex = 0;
}

// ---------------------------------------------------------------------------

void TestTool::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
  }

  if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
    mTileIndex++;
    mTileIndex = mTileIndex % 16;
  }

  if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
    if(!mTestCreated) {
      //createTestButton();
      //createTestObj();
      //createTestMap();
      // createFont();
      mTestCreated = true;

      // createTile();


      //RenderEngine::getInstance()->drawLine(Vector3(0,0,0), Vector3(100,100,0));
    }



    createTile();

    //createFont();

    //Vector2 mouse(Input::getMousePosition());

    // VAR(f32, mouse.x);
    // VAR(f32, mouse.y);

    //Vector3 world = mCamera->screenToWorld(mouse);

    // VAR(f32, world.x);
    // VAR(f32, world.y);

    //File::readFile("resources/shaders/vertex.shader");



  }
}

// ---------------------------------------------------------------------------

void TestTool::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
