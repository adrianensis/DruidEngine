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
#include "Array.h"

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

void TestTool::createBrush() {
  Vector2 size(100,100);

  Texture* texture = Memory::allocate<Texture>();
  texture->init("resources/terrain.png");

  Material* material = Memory::allocate<Material>();
  material->init();
  material->setTexture(texture);
  material->setShader(Shader::getDefaultShader());

  mBrush = Memory::allocate<GameObject>();
  mBrush->init();

  mBrush->getTransform()->setLocalPosition(Vector3(0,0,0));
  mBrush->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mBrush->addComponent<Renderer>(renderer);

  renderer->setLayer(1);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  // renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
  // renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
  // renderer->setAnimation("idle");

  //renderer->setLineMode(true);

  // RigidBody* rigidBody = Memory::allocate<RigidBody>();
  // mBrush->addComponent<RigidBody>(rigidBody);
  //
  // Collider* collider = Memory::allocate<Collider>();
  // mBrush->addComponent<Collider>(collider);
  // collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(mBrush);
}

// ---------------------------------------------------------------------------

void TestTool::createTestButton() {
  mTestButton = UI::getInstance()->createButton(Vector3(600,0,0), Vector2(400,100));

  mTestButton->setOnPressedCallback([&]() {

  });

  getGameObject()->getScene()->addGameObject(mTestButton);
}

// ---------------------------------------------------------------------------

void TestTool::createAtlas(){

  f32 size = 50.0f;

  f32 screenOffset = 300;

  // GameObject* tile = Memory::allocate<GameObject>();
  // tile->init();
  //
  // tile->getTransform()->setLocalPosition(Vector3(0,0,0));
  // tile->getTransform()->setScale(Vector3(size.x, size.y,1));

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

  u32 atlasSzie = tilesCount;

  FOR_RANGE(i, 0, atlasSzie){
    //mGrid->set(i, Memory::allocate<Array<UIButton*>>());
    //mGrid->get(i)->init(mGridSize);
    FOR_RANGE(j, 0, atlasSzie){

      UIButton* tile = UI::getInstance()->createButton(Vector3((i - (atlasSzie/2.0f))*size + screenOffset*2.7f,(j - (atlasSzie/2.0f))*size - screenOffset,0), Vector2(size,size));
      getGameObject()->getScene()->addGameObject(tile);

      Renderer* renderer = tile->getRenderer();
      renderer->setMaterial(mMaterial);
      renderer->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);

      tile->setOnPressedCallback([=, brush = mBrush]() {
        brush->getComponents<Renderer>()->get(0)->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);
      });

      //mGrid->get(i)->set(j, cellButton);

    }
  }

  //Renderer* renderer = Memory::allocate<Renderer>();
  //tile->addComponent<Renderer>(renderer);

  // Vector2 mouse(Input::getMousePosition());
  //
  // VAR(f32, mouse.x);
  // VAR(f32, mouse.y);
  //
  // Vector3 world = mCamera->screenToWorld(mouse);
  //
  // VAR(f32, world.x);
  // VAR(f32, world.y);

  //renderer->setPositionOffset(world/*Vector3((i - tilesCount/2.0f) * size.x, (j - tilesCount/2.0f) * size.y,0)*/);

  //renderer->setMesh(Mesh::getRectangle());
  //renderer->setMaterial(mMaterial);

  //renderer->setRegion(mTileIndex/tilesCount, mTileIndex/tilesCount, tileTextureSize, tileTextureSize);

  //getGameObject()->getScene()->addGameObject(tile);
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
  mBrush = nullptr;
  mTestButton = nullptr;
  mTestCreated = false;
  mTexture = nullptr;
  mMaterial = nullptr;

  mCamera = nullptr;
  mCameraTransform = nullptr;

  mTileIndex = 0;

  mGridSize = 50;
}

// ---------------------------------------------------------------------------

void TestTool::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    mCameraTransform = mCamera->getGameObject()->getTransform();
  }

  if(! mGrid){
    mGrid = Memory::allocate<Array<Array<UIButton*>*>>();
    //mGrid->init(mGridSize);

    f32 halfGridSize = mGridSize/2.0f;

    f32 size = 100;

    FOR_RANGE(i, 0, mGridSize){
      //mGrid->set(i, Memory::allocate<Array<UIButton*>>());
      //mGrid->get(i)->init(mGridSize);
      FOR_RANGE(j, 0, mGridSize){

        // UIButton* cellButton = UI::getInstance()->createButton(Vector3((i - halfGridSize)*size,(j - halfGridSize)*size,0), Vector2(size,size));
        // getGameObject()->getScene()->addGameObject(cellButton);
        //
        // Renderer* renderer = cellButton->getRenderer();
        //
        // cellButton->setOnPressedCallback([&, r = renderer]() {
        //
        // });

        //mGrid->get(i)->set(j, cellButton);

      }
    }
  }

  if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
    mTileIndex++;
    mTileIndex = mTileIndex % 16;
  }

  if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
    if(!mTestCreated) {
      mTestCreated = true;
      createBrush();
      createAtlas();
    }
  }

  f32 movement = 1000.0f * Time::getDeltaTimeSeconds();

  if(Input::isKeyPressed(GLFW_KEY_UP)){
    mCameraTransform->translate(Vector3(0,movement,0));

  }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
    mCameraTransform->translate(Vector3(0,-movement,0));

  }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    mCameraTransform->translate(Vector3(-movement,0,0));

  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    mCameraTransform->translate(Vector3(movement,0,0));

  }

  Vector2 mouse(Input::getMousePosition());

  // VAR(f32, mouse.x);
  // VAR(f32, mouse.y);

  Vector3 world = mCamera->screenToWorld(mouse);

  // VAR(f32, world.x);
  // VAR(f32, world.y);

  if(mBrush) mBrush->getTransform()->setLocalPosition(world);
}

// ---------------------------------------------------------------------------

void TestTool::terminate() {

  // FOR_RANGE(i, 0, mGridSize){
  //   FOR_RANGE(j, 0, mGridSize){
  //     Memory::free<UIButton>(mGrid->get(i)->get(j));
  //   }
  //   Memory::free<Array<UIButton*>>(mGrid->get(i));
  // }

  //mGrid = Memory::allocate<Array<Array<UIButton*>*>>();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
