#include "MapEditor.h"
#include "Log.h"

#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Vector2.h"
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

#include "Settings.h"

namespace DE {

// ---------------------------------------------------------------------------

MapEditor::CellData::CellData() : DE_Class(){

}

// ---------------------------------------------------------------------------

MapEditor::CellData::~CellData(){

}

// ---------------------------------------------------------------------------

MapEditor::MapEditor() : Script(){

}

// ---------------------------------------------------------------------------

MapEditor::~MapEditor() = default;

// ---------------------------------------------------------------------------

void MapEditor::createPlayer() {
  Vector2 size(100*1.5f,100*1.5f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage.png");

  mPlayer = Memory::allocate<GameObject>();
  mPlayer->init();

  mPlayer->getTransform()->setLocalPosition(Vector3(-300,-100,0));
  mPlayer->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mPlayer->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,0), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->addAnimation("run", Animation::create(6, true, false, Vector2(0,0.5), 1.0f/6.0f, 1.0f/2.0f, 10));
  renderer->setAnimation("idle");

  //renderer->setLineMode(true);
  renderer->setLayer(4);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mPlayer->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mPlayer->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  getGameObject()->getScene()->addGameObject(mPlayer);
}

// ---------------------------------------------------------------------------

void MapEditor::createBrush() {
  Vector2 size(mTileSize/2.0f,mTileSize/2.0f);

  mBrush = Memory::allocate<GameObject>();
  mBrush->init();

  mBrush->getTransform()->setLocalPosition(Vector3(0,0,0));
  mBrush->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mBrush->addComponent<Renderer>(renderer);

  renderer->setLayer(2);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(mMaterial);

  getGameObject()->getScene()->addGameObject(mBrush);
}

// ---------------------------------------------------------------------------

void MapEditor::createTile(f32 x, f32 y) {
  // if(mBrush){
    Vector2 size(mTileSize,mTileSize);

    mTile = Memory::allocate<GameObject>();
    mTile->init();

    if(x >= Settings::getInstance()->getF32("scene.size")){
      ECHO("")
      ECHO("Out of scene")
      VAR(f32, x)
      ECHO("")
    }

    mTile->getTransform()->setLocalPosition(Vector3(x,y,0));
    mTile->getTransform()->setScale(Vector3(size.x,size.y,1));

    Renderer* renderer = Memory::allocate<Renderer>();
    mTile->addComponent<Renderer>(renderer);

    renderer->setLayer(1);

    renderer->setMesh(Mesh::getRectangle());
    renderer->setMaterial(mMaterial);

    // renderer->setLineMode(true);

    // RigidBody* rigidBody = Memory::allocate<RigidBody>();
    // mTile->addComponent<RigidBody>(rigidBody);
    //
    // Collider* collider = Memory::allocate<Collider>();
    // mTile->addComponent<Collider>(collider);
    // collider->setSize(size.x,size.y);

    mTile->setIsStatic(true);

    getGameObject()->getScene()->addGameObject(mTile);
  // }
}

// ---------------------------------------------------------------------------

void MapEditor::createAtlas(){

  f32 size = 0.06;

  f32 screenOffset = 0.4f;

  Vector2 atlasSize = Vector2(16,10);
  Vector2 atlasTextureSize = Vector2(1.0f/atlasSize.x, 1.0f/atlasSize.y);

  FOR_RANGE(i, 0, atlasSize.x){
    FOR_RANGE(j, 0, atlasSize.y){

      UIButton* tile = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3((i - (atlasSize.x/2.0f))*size + screenOffset,((atlasSize.y/2.0f) - j)*size - screenOffset*1.5f,0), Vector2(size,size));

      Renderer* renderer = tile->getRenderer();
      renderer->setMaterial(mMaterial);
      renderer->setRegion(i/atlasSize.x, j/atlasSize.y, atlasTextureSize.x, atlasTextureSize.y);
      renderer->setLayer(2);

      tile->setOnPressedCallback([&, self = tile, brush = mBrush]() {
        Renderer* buttonRenderer = self->getRenderer();
        brush->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x, buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x, buttonRenderer->getRegionSize().y);
      });
    }
  }
}

// ---------------------------------------------------------------------------

void MapEditor::init(){
  mTransform = getGameObject()->getTransform();
  mPlayer = nullptr;
  mBrush = nullptr;
  mTestCreated = false;
  mTexture = nullptr;
  mMaterial = nullptr;

  mCamera = nullptr;
  mCameraTransform = nullptr;

  mTileIndex = 0;

  mGridSize = 50;
  mTileSize = 100;

  mTile = nullptr;

  mZoom = 1;
}

// ---------------------------------------------------------------------------

void MapEditor::firstStep(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    mCameraTransform = mCamera->getGameObject()->getTransform();
  }

  if(!mMaterial){
    mMaterial = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");
  }

  createBrush();
  createAtlas();

  f32 textureCoord = 0;

  if(!mGrid){
    mGrid = Memory::allocate<Array<Array<CellData*>*>>();
    mGrid->init(mGridSize);

    f32 halfGridSize = mGridSize/2.0f;

    FOR_RANGE(i, 0, mGridSize){
      mGrid->set(i, Memory::allocate<Array<CellData*>>());
      mGrid->get(i)->init(mGridSize);
      FOR_RANGE(j, 0, mGridSize){
        CellData* cellData = Memory::allocate<CellData>();
        mGrid->get(i)->set(j, cellData);
      }
    }
  }
}

// ---------------------------------------------------------------------------

void MapEditor::step(){

  Vector2 mouse(Input::getMousePosition());
  Vector3 world = mCamera->screenToWorld(mouse);

  Vector3 clampedPosition(std::roundf(world.x/mTileSize)*mTileSize, std::roundf(world.y/mTileSize)*mTileSize,0);

  f32 halfSize = (mGridSize*mTileSize/2.0f);

  Vector2 gridPosition(clampedPosition.x/mTileSize+mGridSize/2.0f,clampedPosition.y/mTileSize+mGridSize/2.0f);

  if(std::fabs(clampedPosition.x) < halfSize && std::fabs(clampedPosition.y) < halfSize){

    CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

    if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
      drawTile(cellData, clampedPosition);
    }

    if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
      removeTile(cellData);
    }
  }

  cameraMovement();

  if(mBrush) mBrush->getTransform()->setLocalPosition(world);

  if(Input::isKeyPressedOnce(GLFW_KEY_S)){
    getGameObject()->getScene()->saveScene("config/scene.conf");
  }

  if(Input::isKeyPressedOnce(GLFW_KEY_L)){
    loadMapIntoGrid();
  }

  if(Input::isKeyPressedOnce(GLFW_KEY_P)){
    if(! mPlayer){
      createPlayer();
    }
  }
}

// ---------------------------------------------------------------------------

void MapEditor::drawTile(CellData* cellData, const Vector3& worldPosition) {
  if(! cellData->tile){
    createTile(worldPosition.x, worldPosition.y);
    cellData->tile = mTile;
  }

  Renderer* renderer = cellData->tile->getComponents<Renderer>()->get(0);
  Renderer* brushRenderer = mBrush->getComponents<Renderer>()->get(0);

  renderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y, brushRenderer->getRegionSize().x, brushRenderer->getRegionSize().y);

}

// ---------------------------------------------------------------------------

void MapEditor::removeTile(CellData* cellData) {
  if(cellData->tile){
    getGameObject()->getScene()->removeGameObject(cellData->tile);
    cellData->tile = nullptr;
  }
}

// ---------------------------------------------------------------------------

void MapEditor::cameraMovement() {
  f32 scroll = Input::getScroll();
  mZoom += std::fabs(scroll)*0.5f*Time::getDeltaTimeSeconds();

  if(scroll == 1){
    mCamera->setZoom(mZoom);
  } else if(scroll == -1){
    mCamera->setZoom(1.0f/mZoom);
  }

  f32 movement = 2000.0f * Time::getDeltaTimeSeconds();

  if(Input::isKeyPressed(GLFW_KEY_UP)){
    mCameraTransform->translate(Vector3(0,movement,0));
  }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
    mCameraTransform->translate(Vector3(0,-movement,0));
  }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    mCameraTransform->translate(Vector3(-movement,0,0));
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    mCameraTransform->translate(Vector3(movement,0,0));
  }

}

// ---------------------------------------------------------------------------

void MapEditor::loadMapIntoGrid() {
  getGameObject()->getScene()->loadScene("config/scene.conf");

  const List<GameObject*>* gameObects = getGameObject()->getScene()->getNewGameObjects();

  FOR_LIST (it, gameObects){
    GameObject* gameObject = it.get();

    if(gameObject->isStatic()){

      Transform* t = gameObject->getTransform();
      Vector3 worldPosition(t->getWorldPosition());
      Vector2 gridPosition(worldPosition.x/mTileSize+mGridSize/2.0f, worldPosition.y/mTileSize+mGridSize/2.0f);

      CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
      cellData->tile = gameObject;
    }

  }
}

// ---------------------------------------------------------------------------

void MapEditor::terminate() {

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
