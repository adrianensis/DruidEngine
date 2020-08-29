#include "GameController.hpp"

#include <Time.hpp>
#include "Log.hpp"

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Input.hpp"
#include "List.hpp"
#include "HashMap.hpp"

#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "MaterialManager.hpp"
#include "Animation.hpp"

#include "Collider.hpp"
#include "RigidBody.hpp"

#include "Scene.hpp"
#include "RenderEngine.hpp"
#include "RenderContext.hpp"

#include "File.hpp"
#include "UI.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "PhysicsEngine.hpp"

#include "ScenesManager.hpp"
#include "EnemyScript.hpp"
#include <cstdlib>

namespace DE {

f32 GameController::smGlobalSpeed = 350;

// ---------------------------------------------------------------------------

GameController::GameController() : Script() {

}

// ---------------------------------------------------------------------------

GameController::~GameController() = default;

// ---------------------------------------------------------------------------

void GameController::createCrossHair() {
	Vector2 size(80, 80);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/crosshair.png");

	mCrossHair = Memory::allocate<GameObject>();
	mCrossHair->init();

	mCrossHair->getTransform()->setLocalPosition(Vector3(0, 0, 0));
	mCrossHair->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mCrossHair->addComponent<Renderer>(renderer);

	renderer->setColor(Vector4(0, 0, 0, 0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	renderer->setLayer(3);

	getGameObject()->getScene()->addGameObject(mCrossHair);
}
// ---------------------------------------------------------------------------

void GameController::createPlayer(f32 x, f32 y) {
	Vector2 size(100 * 1.5f, 100 * 1.5f);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage2.png");

	mPlayer = Memory::allocate<GameObject>();
	mPlayer->init();

	mPlayer->getTransform()->setLocalPosition(Vector3(x, y, 0));
	mPlayer->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mPlayer->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	renderer->setLayer(1);

	Vector2 atlasSize = Vector2(8, 16);

	renderer->addAnimation("attack",
			Animation::create(6, true, false, Vector2(0, 0.0f / 3.0f), 1.0f / 7.0f, 1.0f / 3.0f, 60));
	renderer->addAnimation("idle",
			Animation::create(2, true, false, Vector2(0, 1.0f / 3.0f), 1.0f / 7.0f, 1.0f / 3.0f, 3));
	renderer->addAnimation("run",
			Animation::create(7, true, false, Vector2(0, 2.0f / 3.0f), 1.0f / 7.0f, 1.0f / 3.0f, 10));
	renderer->setAnimation("idle");

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	mPlayer->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	mPlayer->addComponent<Collider>(collider);
	collider->setSize(size.x / 1.5f, size.y);

	mPlayerScript = Memory::allocate<PlayerScript>();
	mPlayer->addComponent<Script>(mPlayerScript);

	getGameObject()->getScene()->addGameObject(mPlayer);
}

// ---------------------------------------------------------------------------

void GameController::createEnemy(f32 x, f32 y) {
	Vector2 size(150, 150);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/demon.png");

	// Vector2 size(100*1.5f,100*1.5f);
	//
	// Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage2.png");

	GameObject* enemy = Memory::allocate<GameObject>();
	enemy->init();

	enemy->getTransform()->setLocalPosition(Vector3(x, y, 0));
	enemy->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	enemy->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	renderer->setLayer(2);

	renderer->addAnimation("death",
			Animation::create(1, true, false, Vector2(0, 0.0f / 1.0f), 1.0f / 4.0f, 1.0f / 2.0f, 7));
	renderer->addAnimation("fly",
			Animation::create(4, true, false, Vector2(0, 1.0f / 2.0f), 1.0f / 4.0f, 1.0f / 2.0f, 7));
	renderer->setAnimation("fly");

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	enemy->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	enemy->addComponent<Collider>(collider);
	collider->setSize(size.x / 4.0f, size.y / 4.0f);
	collider->setIsSolid(false);

	EnemyScript* script = Memory::allocate<EnemyScript>();
	script->setElement(Element::ICE);
	enemy->addComponent<Script>(script);

	getGameObject()->getScene()->addGameObject(enemy);
}

// ---------------------------------------------------------------------------

UIButton* GameController::createBook(f32 x, f32 y, const Vector4 &color, Element element, f32 size) {
	// Vector2 size(300,300);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/book.png");

	// GameObject* book = Memory::allocate<GameObject>();
//	UIButton* book = UI::getInstance()->createButton(getGameObject()->getScene(), Vector2(x, y), Vector2(size, size),
//			5);
	// book->init();

	// book->getTransform()->setLocalPosition(Vector3(x,y,0));
	// book->getTransform()->setScale(Vector3(size.x,size.y,1));
	// book->getTransform()->setScale(Vector3(size,size,1));
	// book->getTransform()->setParent(getGameObject()->getTransform());

	// Renderer* renderer = Memory::allocate<Renderer>();
	// book->addComponent<Renderer>(renderer);

//	book->setOnPressedCallback([&, c = color, e = element, self = book]() {
//		VAR(f32, c.x)
//		VAR(f32, c.y)
//		VAR(f32, c.z)
//
//		((PlayerScript*) mPlayer->getComponents<Script>()->get(0))->setBookColor(c);
//		((PlayerScript*) mPlayer->getComponents<Script>()->get(0))->setElement(e);
//
//		mSelectedBook = self;
//
//		FOR_ARRAY(i, mBookSelector)
//		{
//			if (mBookSelector->get(i) == mSelectedBook) {
//				mBookSelector->get(i)->getTransform()->setScale(Vector3(0.3f, 0.3f, 1));
//			} else {
//				mBookSelector->get(i)->getTransform()->setScale(Vector3(0.2f, 0.2f, 1));
//			}
//		}
//
//	});

//	Renderer* renderer = book->getRenderer();
//
//	renderer->setMesh(Mesh::getRectangle());
//	renderer->setMaterial(material);
//
//	// renderer->setLayer(5);
//	renderer->setColor(color);
//
//	renderer->addAnimation("idle", Animation::create(6, true, false, Vector2(0, 0), 1.0f / 6.0f, 1.0f / 1.0f, 6));
//	renderer->setAnimation("idle");
//
//	// getGameObject()->getScene()->addGameObject(book);

	return nullptr;
}

// ---------------------------------------------------------------------------

void GameController::createBackground(f32 x, f32 y) {
	Vector2 size(2400, 1000);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/background.png");

	mBackground = Memory::allocate<GameObject>();
	mBackground->init();
	mBackground->setIsStatic(true);

	mBackground->getTransform()->setLocalPosition(Vector3(x, y, 0));
	mBackground->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mBackground->addComponent<Renderer>(renderer);

	renderer->setColor(Vector4(0, 0, 0, 1.0f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	renderer->setLayer(0);

	getGameObject()->getScene()->addGameObject(mBackground);
}

// ---------------------------------------------------------------------------

GameObject* GameController::createPhysicFloorTile(f32 x, f32 y) {
	Vector2 size(100, 100);

	// Material* material = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	GameObject* floorTile = Memory::allocate<GameObject>();
	floorTile->init();
	floorTile->setIsStatic(true);

	floorTile->getTransform()->setLocalPosition(Vector3(x, y, 0));
	floorTile->getTransform()->setScale(Vector3(size.x, size.y, 1));

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	floorTile->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	floorTile->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);

	getGameObject()->getScene()->addGameObject(floorTile);

	return floorTile;
}
// ---------------------------------------------------------------------------

GameObject* GameController::createRenderFloorTile(f32 x, f32 y) {
	Vector2 size(mRenderTileSize, 600);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/bridge.png");

	GameObject* floorTile = Memory::allocate<GameObject>();
	floorTile->init();
	floorTile->setIsStatic(false);

	floorTile->getTransform()->setLocalPosition(Vector3(x, y, 0));
	floorTile->getTransform()->setScale(Vector3(size.x, size.y, 1));

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

void GameController::init() {

}

// ---------------------------------------------------------------------------

void GameController::firstStep() {

	createPlayer(-500, 0);
	createCrossHair();

	f32 range = 24;
	mPhysicFloor = Memory::allocate<Array<GameObject*>>();
	mPhysicFloor->init(range);

	FOR_RANGE(i, -(range/2.0f), (range/2.0f)) {
		mPhysicFloor->set(i + (range / 2.0f), createPhysicFloorTile(i * 100, -200));
	}

	mRenderFloor = Memory::allocate<List<GameObject*>>();
	mRenderFloor->init();

	mRenderTileSize = 500;
	mRenderTilesAmount = 7;

	FOR_RANGE(i, -(mRenderTilesAmount/2.0f), (mRenderTilesAmount/2.0f)) {
		mRenderFloor->pushBack(createRenderFloorTile(i * mRenderTileSize, -450));
	}

	mRecycledTiles = Memory::allocate<List<GameObject*>>();
	mRecycledTiles->init();

	createBackground(0, 200);

//	mTextMana = UI::getInstance()->createText(getGameObject()->getScene(), Vector2(-900, 650), Vector2(50, 50),
//			"MANA: " + std::to_string(mPlayerScript->getMana()), 5);

	mEnemySpawnTimeCount = 0;
	mEnemySpawnTime = 1.0f;

	mBookSelector = Memory::allocate<Array<UIButton*>>();
	mBookSelector->init(3);
	mBookSelector->set(0, createBook(-0.8f, -0.4f, Vector4(-0.7f, 0.2f, 0.3f, 1), Element::ICE, 0.2f));
	mBookSelector->set(1, createBook(-0.65f, -0.6f, Vector4(0, 0, 0, 1), Element::FIRE, 0.3f));
	mBookSelector->set(2, createBook(-0.8f, -0.8f, Vector4(-0.9f, 0.3f, 0, 1), Element::WIND, 0.2f));

	mSelectedBook = mBookSelector->get(0);
}

// ---------------------------------------------------------------------------

void GameController::step() {

//	if (mPlayerScript->isManaChanged()) {
//		mTextMana->setText("MANA: " + std::to_string(mPlayerScript->getMana()));
//	}

	Vector2 mouse = Input::getInstance()->getMousePosition();

	Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(
			mouse);

	if (mEnemySpawnTimeCount >= mEnemySpawnTime) {
		// mTextMana->setIsActive(! mTextMana->isActive());
		mEnemySpawnTimeCount = 0;

		createEnemy(1200, (std::rand() % 500) + 100);
	}

	mCrossHair->getTransform()->setLocalPosition(world);

	mEnemySpawnTimeCount += Time::getInstance()->getDeltaTimeSeconds();

	// f32 movement = 350;
	f32 movementDirection = 0;
	Vector3 floorMovement = Vector3(0, 0, 0);

	floorMovement.x = -smGlobalSpeed * Time::getInstance()->getDeltaTimeSeconds();
	floorMovement.y = 0;
	movementDirection = -1; // TILES MOVES TO LEFT

	if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_1)) {
		mBookSelector->get(0)->onPressed();
	} else if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_2)) {
		mBookSelector->get(1)->onPressed();
	} else if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_3)) {
		mBookSelector->get(2)->onPressed();
	}

	if (mPlayer) {

		if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)) {

			// Vector2 mouse = Input::getInstance()->getMousePosition();
			//
			// Vector3 world = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0)->screenToWorld(mouse);
			//
			// createProjectile(mPlayer->getTransform()->getWorldPosition().x, mPlayer->getTransform()->getWorldPosition().y, world.x, world.y);

		}
		if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)) {

		}
		if (Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT)) {
			// floorMovement.x = movement * Time::getInstance()->getDeltaTimeSeconds();
			// floorMovement.y = 0;
			// movementDirection = 1; // TILES MOVES TO RIGHT

		} else if (Input::getInstance()->isKeyPressed(GLFW_KEY_RIGHT)) {
			// floorMovement.x = -movement * Time::getInstance()->getDeltaTimeSeconds();
			// floorMovement.y = 0;
			// movementDirection = -1; // TILES MOVES TO LEFT
		} else if (Input::getInstance()->isKeyPressedOnce(GLFW_KEY_SPACE)) {

		} else {
			// floorMovement.x = 0;
			// floorMovement.y = 0;
			// movementDirection = 0;
		}

		if (mRenderFloor && movementDirection != 0) {

			FOR_LIST(it, mRenderFloor) {
				GameObject* tile = it.get();
				Renderer* renderer = tile->getComponents<Renderer>()->get(0);

				tile->getTransform()->translate(floorMovement);

				if (renderer->isOutOfCamera()) {

					bool checkDirectionAndPosition = (movementDirection == -1
							&& tile->getTransform()->getWorldPosition().x < 0);

					if (checkDirectionAndPosition) {
						mRenderFloor->remove(it);
						mRecycledTiles->pushBack(tile);
					}
				}
			}

			if (mRecycledTiles->getLength() > 0) {
				u32 count = 0;
				FOR_LIST(it, mRecycledTiles)
				{
					GameObject* tile = it.get();

					mRenderFloor->pushBack(tile);

					tile->getTransform()->translate(
							Vector3(-movementDirection * mRenderFloor->getLength() * mRenderTileSize, 0, 0));

					count++;
				}
			}

			mRecycledTiles->clear();
		}
	}
}

// ---------------------------------------------------------------------------

void GameController::terminate() {

	Memory::free<Array<UIButton*>>(mBookSelector);

}

// ---------------------------------------------------------------------------

} /* namespace DE */
