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
  Vector2 size(mTileSize,mTileSize);

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

  renderer->setLayer(2);

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

void TestTool::createTile(f32 x, f32 y) {
  if(mBrush){
    Vector2 size(mTileSize,mTileSize);

    mTestTile = Memory::allocate<GameObject>();
    mTestTile->init();

    mTestTile->getTransform()->setLocalPosition(Vector3(x,y,0));
    mTestTile->getTransform()->setScale(Vector3(size.x,size.y,1));

    Renderer* renderer = Memory::allocate<Renderer>();
    mTestTile->addComponent<Renderer>(renderer);

    renderer->setLayer(1);

    //renderer->setColor(Vector4(0,0,0,0.7f));

    renderer->setMesh(Mesh::getRectangle());
    renderer->setMaterial(mMaterial);

    f32 tilesCount = 16;
    f32 tileTextureSize = 1.0f/tilesCount;

    renderer->setRegion(mAtlasIndexX/tilesCount, mAtlasIndexY/tilesCount, tileTextureSize, tileTextureSize);

    // renderer->setLineMode(true);

    // RigidBody* rigidBody = Memory::allocate<RigidBody>();
    // mTestTile->addComponent<RigidBody>(rigidBody);
    //
    // Collider* collider = Memory::allocate<Collider>();
    // mTestTile->addComponent<Collider>(collider);
    // collider->setSize(size.x,size.y);

    getGameObject()->getScene()->addGameObject(mTestTile);
  }
}

// ---------------------------------------------------------------------------

void TestTool::createTestButton() {
  mTestButton = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3(600,0,0), Vector2(400,100));

  mTestButton->setOnPressedCallback([&]() {

  });
}

// ---------------------------------------------------------------------------

void TestTool::createAtlas(){

  f32 size = mTileSize;

  f32 screenOffset = 500;

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

      UIButton* tile = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3((i - (atlasSzie/2.0f))*size + screenOffset*2.7f,(j - (atlasSzie/2.0f))*size - screenOffset,0), Vector2(size,size));

      Renderer* renderer = tile->getRenderer();
      renderer->setMaterial(mMaterial);
      renderer->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);
      renderer->setLayer(1);

      tile->setOnPressedCallback([=, brush = mBrush]() {
        brush->getComponents<Renderer>()->get(0)->setRegion(i/tilesCount, j/tilesCount, tileTextureSize, tileTextureSize);
        mAtlasIndexX = i;
        mAtlasIndexY = j;

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

  UIText* text = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(world.x,world.y), Vector2(50,50), std::string("Hello stranger..."));
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
  mTileSize = 100;

  mTestTile = nullptr;
}

// ---------------------------------------------------------------------------

void TestTool::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    mCameraTransform = mCamera->getGameObject()->getTransform();
  }

  if(!mTestCreated) {
    mTestCreated = true;
    createBrush();
    createAtlas();
  }

  if(! mGrid){
    mGrid = Memory::allocate<Array<Array<CellData*>*>>();
    mGrid->init(mGridSize);

    f32 halfGridSize = mGridSize/2.0f;

    FOR_RANGE(i, 0, mGridSize){
      mGrid->set(i, Memory::allocate<Array<CellData*>>());
      mGrid->get(i)->init(mGridSize);
      FOR_RANGE(j, 0, mGridSize){

        CellData* cellData = Memory::allocate<CellData>();
        mGrid->get(i)->set(j, cellData);

        cellData->button = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3((i - halfGridSize)*mTileSize,(j - halfGridSize)*mTileSize,0), Vector2(mTileSize,mTileSize));

        Vector3 cellPosition = cellData->button->getTransform()->getLocalPosition();

        cellData->button->setOnPressedCallback([self = this, x = cellPosition.x, y = cellPosition.y, grid = mGrid, i=i, j=j ]() {

          ECHO("Click")
          if(!grid->get(i)->get(j)->tile){
            self->createTile(x,y);
            grid->get(i)->get(j)->tile = self->mTestTile;
          }else{
            ECHO("Already set")
          }

          grid->get(i)->get(j)->textureRegion = Vector2(self->mAtlasIndexX, self->mAtlasIndexY);

          f32 tilesCount = 16;
          f32 tileTextureSize = 1.0f/tilesCount;

          grid->get(i)->get(j)->tile->getComponents<Renderer>()->get(0)->setRegion(self->mAtlasIndexX/tilesCount, self->mAtlasIndexY/tilesCount, tileTextureSize, tileTextureSize);

        });
      }
    }

    File::readFile("config/map.conf", [&](std::ifstream& file) {

      std::string lineString;

      std::regex regexTile("([0-9]+),([0-9]+) ([0-9]+),([0-9]+)");

      while(std::getline(file,lineString)){

        std::smatch matchTile;

        std::regex_search(lineString, matchTile, regexTile);

        if(!matchTile.empty()){

          u32 i = std::stoi(matchTile[1]);
          u32 j = std::stoi(matchTile[2]);
          u32 atlasIndexX = std::stoi(matchTile[3]);
          u32 atlasIndexY = std::stoi(matchTile[4]);

          mGrid->get(i)->get(j)->textureRegion = Vector2(atlasIndexX, atlasIndexY);
          mAtlasIndexX = atlasIndexX;
          mAtlasIndexY = atlasIndexY;

          Vector3 cellPosition = mGrid->get(i)->get(j)->button->getTransform()->getLocalPosition();
          createTile(cellPosition.x,cellPosition.y);

          mGrid->get(i)->get(j)->tile = mTestTile;

        }
      }
    });
  }

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)){
    mTileIndex++;
    mTileIndex = mTileIndex % 16;
  }

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

    //
    // if(!mTestTile){
    //   //createTile();
    // }
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

  }else if(Input::isKeyPressedOnce(GLFW_KEY_S)){

    File::writeFile("config/map.conf", [&](std::ofstream& file) {

      ECHO("Map saved!")

      FOR_RANGE(i, 0, mGridSize){
        FOR_RANGE(j, 0, mGridSize){

          if(mGrid->get(i)->get(j)->tile){
            file << i << "," << j << " ";
            file << mGrid->get(i)->get(j)->textureRegion.x << "," << mGrid->get(i)->get(j)->textureRegion.y << std::endl;
          }
        }
      }
    });

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
