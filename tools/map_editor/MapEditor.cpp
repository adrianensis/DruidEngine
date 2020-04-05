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

  mAtlasIndexX = 0;
  mAtlasIndexY = 0;
  renderer->setRegion(mAtlasIndexX/mAtlasSize.x, mAtlasIndexY/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);

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

    //renderer->setColor(Vector4(0,0,0,0.7f));

    renderer->setMesh(Mesh::getRectangle());
    renderer->setMaterial(mMaterial);

    // renderer->setRegion(mAtlasIndexX/mAtlasSize.x, mAtlasIndexY/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);

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

  f32 size = 0.04;

  f32 screenOffset = 0.5f;

  FOR_RANGE(i, 0, mAtlasSize.x){
    FOR_RANGE(j, 0, mAtlasSize.y){

      UIButton* tile = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3((i - (mAtlasSize.x/2.0f))*size + screenOffset,((mAtlasSize.y/2.0f) - j)*size - screenOffset,0), Vector2(size,size));

      Renderer* renderer = tile->getRenderer();
      renderer->setMaterial(mMaterial);
      renderer->setRegion(i/mAtlasSize.x, j/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);
      renderer->setLayer(2);

      tile->setOnPressedCallback([=, brush = mBrush]() {
        brush->getComponents<Renderer>()->get(0)->setRegion(i/mAtlasSize.x, j/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);
        mAtlasIndexX = i;
        mAtlasIndexY = j;
      });
    }
  }

  UIButton* button = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3(0,0.8,0), Vector2(0.3,0.08));
  Renderer* r = button->getRenderer();
  r->setMaterial(mMaterial);
  r->setRegion(1.0f/mAtlasSize.x, 11.0f/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);
  r->setLayer(2);

  button = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3(-550 + 350,600,0), Vector2(300,80));
  r = button->getRenderer();
  r->setMaterial(mMaterial);
  r->setRegion(1.0f/mAtlasSize.x, 11.0f/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);
  r->setLayer(2);
}

// ---------------------------------------------------------------------------

void MapEditor::init(){
  mTransform = getGameObject()->getTransform();
  mBrush = nullptr;
  mTestCreated = false;
  mTexture = nullptr;
  mMaterial = nullptr;

  mCamera = nullptr;
  mCameraTransform = nullptr;

  mTileIndex = 0;

  mGridSize = 30;
  mTileSize = 100;

  mTile = nullptr;

  mZoom = 1;

  mAtlasSize = Vector2(16,10);
  mAtlasTextureSize = Vector2(1.0f/mAtlasSize.x, 1.0f/mAtlasSize.y);
}

// ---------------------------------------------------------------------------

void MapEditor::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    mCameraTransform = mCamera->getGameObject()->getTransform();
  }

  if(!mTestCreated) {

    mTestCreated = true;

    if(!mTexture){

      mTexture = Memory::allocate<Texture>();
      mTexture->init("resources/tiles.png");

      mMaterial = Memory::allocate<Material>();
      mMaterial->init();
      mMaterial->setTexture(mTexture);
      mMaterial->setShader(Shader::getDefaultShader());
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

          if(! cellData->tile){
            createTile((i-halfGridSize)*mTileSize, (j-halfGridSize)*mTileSize);
            cellData->tile = mTile;
          }

          cellData->textureRegion = Vector2(mAtlasIndexX, mAtlasIndexY);
          cellData->tile->getComponents<Renderer>()->get(0)->setRegion(textureCoord/mAtlasSize.x, textureCoord/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);

          // UIText* text = UI::getInstance()->createText(getGameObject()->getScene(), Vector3((i-halfGridSize)*mTileSize, (j-halfGridSize)*mTileSize,0), Vector2(10,10), std::to_string(i)+","+std::to_string(j));
          //
          // text->setIsStatic(true);
          //
          // text->getRenderer()
          // ->setAffectedByProjection(true);
          // if(textureCoord == 0) textureCoord = 8;
          // else textureCoord = 0;
        }
      }
    }
  }

  Vector2 mouse(Input::getMousePosition());
  Vector3 world = mCamera->screenToWorld(mouse);

  Vector3 clampedPosition(std::roundf(world.x/mTileSize)*mTileSize, std::roundf(world.y/mTileSize)*mTileSize,0);

  f32 halfSize = (mGridSize*mTileSize/2.0f);

  Vector2 gridPosition(clampedPosition.x/mTileSize+mGridSize/2.0f,clampedPosition.y/mTileSize+mGridSize/2.0f);

  if(std::fabs(clampedPosition.x) < halfSize && std::fabs(clampedPosition.y) < halfSize){

    CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

    if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){

        if(! cellData->tile){

          createTile(clampedPosition.x, clampedPosition.y);
          cellData->tile = mTile;
        }

        cellData->textureRegion = Vector2(mAtlasIndexX, mAtlasIndexY);
        cellData->tile->getComponents<Renderer>()->get(0)->setRegion(cellData->textureRegion.x/mAtlasSize.x, cellData->textureRegion.y/mAtlasSize.y, mAtlasTextureSize.x, mAtlasTextureSize.y);

    }

    if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
      if(cellData->tile){
        getGameObject()->getScene()->removeGameObject(cellData->tile);
        cellData->tile = nullptr;
      }
    }
  }

  f32 scroll = Input::getScroll();
  mZoom += std::fabs(scroll)*0.5f*Time::getDeltaTimeSeconds();

  if(scroll == 1){
    mCamera->setZoom(mZoom);
  } else if(scroll == -1){
    mCamera->setZoom(1.0f/mZoom);
  }

  f32 movement = 5000.0f * Time::getDeltaTimeSeconds();

  if(Input::isKeyPressed(GLFW_KEY_UP)){
    mCameraTransform->translate(Vector3(0,movement,0));
  }else if(Input::isKeyPressed(GLFW_KEY_DOWN)){
    mCameraTransform->translate(Vector3(0,-movement,0));
  }else if(Input::isKeyPressed(GLFW_KEY_LEFT)){
    mCameraTransform->translate(Vector3(-movement,0,0));
  }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
    mCameraTransform->translate(Vector3(movement,0,0));
  }

  if(mBrush) mBrush->getTransform()->setLocalPosition(world);
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
