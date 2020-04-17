#include "GameController.h"
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
#include "EnemyScript.h"

namespace DE {

// ---------------------------------------------------------------------------

GameController::GameController() : Script(){

}

// ---------------------------------------------------------------------------

GameController::~GameController() = default;

// ---------------------------------------------------------------------------

void GameController::createPlayer(f32 x, f32 y){
  Vector2 size(100*1.5f,100*1.5f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage2.png");

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

  // renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,14.0f/16.0f), 1.0f/8.0f, 1.0f/16.0f, 10));
  renderer->addAnimation("idle", Animation::create(2, true, false, Vector2(0,0.0f/2.0f), 1.0f/7.0f, 1.0f/2.0f, 3));
  renderer->addAnimation("run", Animation::create(7, true, false, Vector2(0,1.0f/2.0f), 1.0f/7.0f, 1.0f/2.0f, 10));
  renderer->setAnimation("idle");

  // renderer->setRegion(0.0f, 14.0f/atlasSize.y, 6.0f/atlasSize.x, 2.0f/atlasSize.y);

  //renderer->setLineMode(true);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  mPlayer->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  mPlayer->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  Script* script = Memory::allocate<PlayerScript>();
	mPlayer->addComponent<Script>(script);

  getGameObject()->getScene()->addGameObject(mPlayer);
}

// ---------------------------------------------------------------------------

void GameController::createEnemy(f32 x, f32 y){
  Vector2 size(100*1.5f,100*1.5f);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage2.png");

  GameObject* enemy = Memory::allocate<GameObject>();
  enemy->init();

  enemy->getTransform()->setLocalPosition(Vector3(x,y,0));
  enemy->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  enemy->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(1);

  Vector2 atlasSize = Vector2(8,16);

  // renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0,14.0f/16.0f), 1.0f/8.0f, 1.0f/16.0f, 10));
  renderer->addAnimation("idle", Animation::create(2, true, false, Vector2(0,0.0f/2.0f), 1.0f/7.0f, 1.0f/2.0f, 3));
  renderer->addAnimation("run", Animation::create(7, true, false, Vector2(0,1.0f/2.0f), 1.0f/7.0f, 1.0f/2.0f, 10));
  renderer->setAnimation("idle");

  // renderer->setRegion(0.0f, 14.0f/atlasSize.y, 6.0f/atlasSize.x, 2.0f/atlasSize.y);

  //renderer->setLineMode(true);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  enemy->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  enemy->addComponent<Collider>(collider);
  collider->setSize(size.x/1.5f,size.y);

  Script* script = Memory::allocate<EnemyScript>();
  enemy->addComponent<Script>(script);

  getGameObject()->getScene()->addGameObject(enemy);
}

// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------

void GameController::createBackground(f32 x, f32 y){
  Vector2 size(2400,1000);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/background.png");

  mBackground = Memory::allocate<GameObject>();
  mBackground->init();
  mBackground->setIsStatic(true);

  mBackground->getTransform()->setLocalPosition(Vector3(x,y,0));
  mBackground->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  mBackground->addComponent<Renderer>(renderer);

  renderer->setColor(Vector4(0,0,0,1.0f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(0);

  getGameObject()->getScene()->addGameObject(mBackground);
}

// ---------------------------------------------------------------------------

GameObject* GameController::createPhysicFloorTile(f32 x, f32 y){
  Vector2 size(100,100);

  // Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

  GameObject* floorTile = Memory::allocate<GameObject>();
  floorTile->init();
  floorTile->setIsStatic(true);

  floorTile->getTransform()->setLocalPosition(Vector3(x,y,0));
  floorTile->getTransform()->setScale(Vector3(size.x,size.y,1));

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  floorTile->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  floorTile->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  getGameObject()->getScene()->addGameObject(floorTile);

  return floorTile;
}
// ---------------------------------------------------------------------------

GameObject* GameController::createRenderFloorTile(f32 x, f32 y){
  Vector2 size(mRenderTileSize,600);

  Material* material = MaterialManager::getInstance()->loadMaterial("resources/bridge.png");

  GameObject* floorTile = Memory::allocate<GameObject>();
  floorTile->init();
  floorTile->setIsStatic(false);

  floorTile->getTransform()->setLocalPosition(Vector3(x,y,0));
  floorTile->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  floorTile->addComponent<Renderer>(renderer);

  //renderer->setColor(Vector4(0,0,0,0.7f));

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(material);

  renderer->setLayer(1);

  // Vector2 atlasSize = Vector2(8,16);

  // renderer->setRegion(5.0f/atlasSize.x, 0/atlasSize.y, 1.0f/atlasSize.x, 1.0f/atlasSize.y);

  getGameObject()->getScene()->addGameObject(floorTile);

  return floorTile;
}

// ---------------------------------------------------------------------------

void GameController::init(){

}

// ---------------------------------------------------------------------------

void GameController::firstStep(){

  createPlayer(-500,0);

  f32 range = 24;
  mPhysicFloor = Memory::allocate<Array<GameObject*>>();
  mPhysicFloor->init(range);

  FOR_RANGE(i, -(range/2.0f), (range/2.0f)){
    mPhysicFloor->set(i + (range/2.0f), createPhysicFloorTile(i*100,-200));
  }

  mRenderFloor = Memory::allocate<List<GameObject*>>();
  mRenderFloor->init();

  mRenderTileSize = 500;
  mRenderTilesAmount = 7;

  FOR_RANGE(i, -(mRenderTilesAmount/2.0f), (mRenderTilesAmount/2.0f)){
    mRenderFloor->pushBack(createRenderFloorTile(i*mRenderTileSize,-450));
  }

  mRecycledTiles = Memory::allocate<List<GameObject*>>();
  mRecycledTiles->init();

  createBackground(0,200);
}

// ---------------------------------------------------------------------------

void GameController::step(){
  if(Input::isKeyPressedOnce(GLFW_KEY_P)){
  } else if(Input::isKeyPressedOnce(GLFW_KEY_F)){
  } else if(Input::isKeyPressedOnce(GLFW_KEY_B)){
  }

  if(mPlayer){
    f32 movement = 600;
    f32 movementDirection = 0;
    Vector3 floorMovement = Vector3(0,0,0);

    if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

      // Vector2 mouse = Input::getMousePosition();
      //
      // Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(mouse);
      //
      // createProjectile(mPlayer->getTransform()->getWorldPosition().x, mPlayer->getTransform()->getWorldPosition().y, world.x, world.y);

    }if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)){

      Vector2 mouse = Input::getMousePosition();

      Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(mouse);

      createEnemy(world.x,-75);

    }if(Input::isKeyPressed(GLFW_KEY_LEFT)){
      // floorMovement.x = movement * Time::getDeltaTimeSeconds();
      // floorMovement.y = 0;
      // movementDirection = 1; // TILES MOVES TO RIGHT


    }else if(Input::isKeyPressed(GLFW_KEY_RIGHT)){
      floorMovement.x = -movement * Time::getDeltaTimeSeconds();
      floorMovement.y = 0;
      movementDirection = -1; // TILES MOVES TO LEFT
    }else if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){

    }else{
      floorMovement.x = 0;
      floorMovement.y = 0;
      movementDirection = 0;
    }

    if(mRenderFloor && movementDirection != 0){

      FOR_LIST(it, mRenderFloor){
        GameObject* tile = it.get();
        Renderer* renderer = tile->getComponents<Renderer>()->get(0);

        tile->getTransform()->translate(floorMovement);

        if(renderer->isOutOfCamera()){

          bool checkDirectionAndPosition =
          (movementDirection == -1 && tile->getTransform()->getWorldPosition().x < 0);

          if(checkDirectionAndPosition){
            mRenderFloor->remove(it);
            mRecycledTiles->pushBack(tile);
          }
        }
      }

      if(mRecycledTiles->getLength() > 0){
        u32 count = 0;
        FOR_LIST(it, mRecycledTiles){
          GameObject* tile = it.get();

          mRenderFloor->pushBack(tile);

          tile->getTransform()->translate(Vector3(-movementDirection * mRenderFloor->getLength() * mRenderTileSize,0,0));

          count++;
        }
      }

      mRecycledTiles->clear();
    }
  }
}

// ---------------------------------------------------------------------------

void GameController::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
