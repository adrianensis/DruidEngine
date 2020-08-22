#include "MapEditor.hpp"
#include "Log.hpp"

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Time2.hpp"
#include "Input.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "Array.hpp"

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

#include "Settings.hpp"
#include "ConfigMap.hpp"

namespace DE {

// ---------------------------------------------------------------------------

MapEditor::CellData::CellData() : DE_Class() {

}

// ---------------------------------------------------------------------------

MapEditor::CellData::~CellData() {
	if (layers) {
		Memory::free<Array<GameObject*>>(layers);
	}
}

// ---------------------------------------------------------------------------

void MapEditor::CellData::addGameObject(GameObject *gameObject, u32 layer) {
	if (!layers) {
		layers = Memory::allocate<Array<GameObject*>>();
		layers->init(10); // MAX LAYERS
	}

	if (layer >= 0 && layer < 10) {
		layers->set(layer, gameObject);
	}
}

void MapEditor::CellData::removeGameObject(u32 layer) {
	if (layers) {
		if (layer >= 0 && layer < 10) {
			layers->set(layer, nullptr);
		}
	}
}

GameObject* MapEditor::CellData::get(u32 layer) {
	GameObject* gameObject = nullptr;

	if (layers) {
		gameObject = layers->get(layer);
	}

	return gameObject;
}


// ---------------------------------------------------------------------------

MapEditor::Brush::Brush() : DE_Class() {

}

// ---------------------------------------------------------------------------

MapEditor::Brush::~Brush() {
	free();
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::init(MapEditor* mapEditor) {

	mMapEditor = mapEditor;

	if(mGrid) free();

	mGrid = Memory::allocate<Array<GameObject*>>();
	mGrid->init(mMaxGridSize);

	Vector2 size(mMapEditor->mGridTileSize / 2.0f, mMapEditor->mGridTileSize / 2.0f);

	if(mBrushCursor) {
		mMapEditor->getGameObject()->getScene()->removeGameObject(mBrushCursor);
	}

	mBrushCursor = Memory::allocate<GameObject>();
	mBrushCursor->init();

	mBrushCursor->getTransform()->setLocalPosition(Vector3(-1.4f / RenderContext::getAspectRatio(), 0.8f, 0));
	mBrushCursor->getTransform()->setScale(Vector3(0.1f / RenderContext::getAspectRatio(), 0.1f, 1));
	mBrushCursor->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = Memory::allocate<Renderer>();
	mBrushCursor->addComponent<Renderer>(renderer);

	renderer->setLayer(mMapEditor->mMapEditorUI.mUILayer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mMapEditor->mMaterial);


	mMapEditor->getGameObject()->getScene()->addGameObject(mBrushCursor);
}

// ---------------------------------------------------------------------------

GameObject* MapEditor::Brush::getTile(u32 i, u32 j){
	return mGrid->get(i * mGridSize.x + j);
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::addTile(GameObject* tile, Vector2 atlasPosition){
	if(mLastIndex < mMaxGridSize){

		// If it's the first selected tile, take it as coordinates origin.
		if(! mGrid->get(0)){
			mOriginAtlasPosition = atlasPosition;
		}

		Vector2 distance(std::abs(atlasPosition.x - mOriginAtlasPosition.x),
				std::abs(atlasPosition.y - mOriginAtlasPosition.y));

		mGridSize.x = std::max(mGridSize.x,distance.x + 1);
		mGridSize.y = std::max(mGridSize.y,distance.y + 1);

		mGrid->set(mLastIndex,tile);
		mLastIndex++;
		tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));
	}
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::free(){
	Memory::free<Array<GameObject*>>(mGrid);
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::clear(){
	FOR_RANGE(i, 0, mLastIndex){
		GameObject* tile = mGrid->get(i);

		if(tile)
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
	}

	mLastIndex = 0;
	mGridSize = Vector2(1,1);
	mOriginAtlasPosition = Vector2(0,0);
	mGrid->clear();
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::setDrawTileSize(f32 size) {
	mDrawTileSize = size;
	//mBrushCursor->getTransform()->setScale(Vector3(mDrawTileSize,mDrawTileSize,1));
}

// ---------------------------------------------------------------------------

MapEditor::MapEditor() : Script() {

}

// ---------------------------------------------------------------------------

MapEditor::~MapEditor() = default;

// ---------------------------------------------------------------------------

void MapEditor::createPlayer() {
	Vector2 size(100 * 1.5f, 100 * 1.5f);

	// Material* material = MaterialManager::getInstance()->loadMaterial("resources/mage.png");

	mPlayer = Memory::allocate<GameObject>();
	mPlayer->init();

	mPlayer->getTransform()->setLocalPosition(Vector3(-300, -100, 0));
	mPlayer->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mPlayer->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	// renderer->setMaterial(material);
	renderer->setMaterial(mMaterial);

	renderer->setLayer(mLayer);

	Vector2 atlasSize = Vector2(8, 16);

	renderer->addAnimation("idle",
			Animation::create(6, true, false, Vector2(0, 14.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->addAnimation("run",
			Animation::create(6, true, false, Vector2(0, 15.0f / atlasSize.y), 1.0f / atlasSize.x, 1.0f / atlasSize.y,
					10));
	renderer->setAnimation("idle");

	//renderer->setLineMode(true);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	mPlayer->addComponent<RigidBody>(rigidBody);

	Collider* collider = Memory::allocate<Collider>();
	mPlayer->addComponent<Collider>(collider);
	collider->setSize(size.x / 1.5f, size.y);

	getGameObject()->getScene()->addGameObject(mPlayer);
}

void MapEditor::destroyPlayer(){
	getGameObject()->getScene()->removeGameObject(mPlayer);
	mPlayer = nullptr;
}

// ---------------------------------------------------------------------------

void MapEditor::resetBrush() {
	mIsPaintMode = false;
	mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(0, 0, 1, 1);
	mBrush.clear();
}

// ---------------------------------------------------------------------------

void MapEditor::createBrush() {
	mBrush.init(this);
	mBrush.setDrawTileSize(mGridTileSize);
}

// ---------------------------------------------------------------------------

GameObject* MapEditor::createTile(f32 x, f32 y) {
	Vector2 size(mBrush.mDrawTileSize, mBrush.mDrawTileSize);

	GameObject* tile = Memory::allocate<GameObject>();
	tile->init();

	tile->getTransform()->setLocalPosition(Vector3(x, y, 0));
	tile->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	tile->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mMaterial);

	renderer->setLayer(mLayer);

	tile->setIsStatic(true);
	tile->setShouldPersist(true);

	getGameObject()->getScene()->addGameObject(tile);

	return tile;
}


// ---------------------------------------------------------------------------

void MapEditor::init() {
	mTransform = getGameObject()->getTransform();

	mConfigMap = Memory::allocate<ConfigMap>();
	mConfigMap->init();

	mConfigMap->readConfigFile("config/editor.conf");

	mGridSize = mConfigMap->getF32("grid.size");
	mGridTileSize = mConfigMap->getF32("grid.tile.size");
}

// ---------------------------------------------------------------------------

void MapEditor::firstStep() {

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mMaterial = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");
	// TODO : brush will need to change material dinamically.
	// OR Destroy brush cursor and create again.

	mGrid = Memory::allocate<Array<Array<CellData*>*>>();
	mGrid->init(mGridSize);

	f32 halfGridSize = mGridSize / 2.0f;

	FOR_RANGE(i, 0, mGridSize){
		mGrid->set(i, Memory::allocate<Array<CellData*>>());
		mGrid->get(i)->init(mGridSize);
		FOR_RANGE(j, 0, mGridSize){
			CellData* cellData = Memory::allocate<CellData>();
			mGrid->get(i)->set(j, cellData);
		}
	}

	loadMapIntoGrid();

	mMapEditorUI.init(this);

	createBrush();

	//createPlayer();
}

// ---------------------------------------------------------------------------

void MapEditor::step() {

	Vector2 mouse(Input::getMousePosition());
	Vector3 world = mCamera->screenToWorld(mouse);

	Vector3 clampedPosition(std::roundf(world.x / mGridTileSize) * mGridTileSize, std::roundf(world.y / mGridTileSize) * mGridTileSize, 0);

	//click(clampedPosition);
	if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)
	|| Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

		if(RenderEngine::getInstance()->getLayersData()->get(mLayer)->mVisible){
			FOR_RANGE(i, 0, mBrush.mGridSize.y){
				FOR_RANGE(j, 0, mBrush.mGridSize.x){
					f32 offset = mGridTileSize;
					click(clampedPosition + Vector3(offset*j, -offset*i, 0), mBrush.getTile(i, j));
				}
			}
		}
	}

//	if (mBrush.mBrushCursor)
//		mBrush.mBrushCursor->getTransform()->setLocalPosition(world);

	cameraZoom();
	processMovement();

	if (Input::isKeyPressedOnce(GLFW_KEY_Z)) {
		if (mLayer < 10)
			mLayer++;
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_X)) {
		if (mLayer > 0)
			mLayer--;
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_ESCAPE)) {
		resetBrush();
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_TAB)) {
		mMapEditorUI.toggleAtlas();
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_S) && Input::isModifierPressed(GLFW_MOD_CONTROL /*| GLFW_MOD_SHIFT*/)) {
		getGameObject()->getScene()->saveScene(getGameObject()->getScene()->getPath());
	}

	mMapEditorUI.update();
}

// ---------------------------------------------------------------------------

void MapEditor::click(const Vector3 &clampedPosition, GameObject* brush) {

	f32 halfSize = (mGridSize * mGridTileSize / 2.0f);
	if (std::fabs(clampedPosition.x) < halfSize && std::fabs(clampedPosition.y) < halfSize) {

		Vector2 gridPosition(clampedPosition.x / mGridTileSize + mGridSize / 2.0f,
				clampedPosition.y / mGridTileSize + mGridSize / 2.0f);

		if (gridPosition.x >= 0 && gridPosition.x < mGridSize && gridPosition.y >= 0 && gridPosition.y < mGridSize) {

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if(mIsPaintMode){
					drawTile(cellData, clampedPosition, brush);
				} else {
					selectTile(cellData);
				}
			}

			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

				if(mIsPaintMode){
					removeTile(cellData);
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------

void MapEditor::addColliderToTile(GameObject *tile) {
	if(tile){
		List<RigidBody*>* rigidBodies = tile->getComponents<RigidBody>();

		if(!rigidBodies || (rigidBodies && rigidBodies->isEmpty())){
			RigidBody* rigidBody = Memory::allocate<RigidBody>();
			tile->addComponent<RigidBody>(rigidBody);

			Collider* collider = Memory::allocate<Collider>();
			tile->addComponent<Collider>(collider);
			collider->setSize(tile->getTransform()->getScale().x, tile->getTransform()->getScale().y);

			getGameObject()->getScene()->updateComponents(tile);
		}
	}
}

// ---------------------------------------------------------------------------

void MapEditor::selectTile(CellData *cellData) {
	if (cellData->get(mLayer)) {
		if(mSelectedTile && mSelectedTile->isActive()){
			mSelectedTile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
		}

		mSelectedTile = cellData->get(mLayer);
		mSelectedTile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));

		mMapEditorUI.updateInspectorOnSelectTile();
	}
}

// ---------------------------------------------------------------------------

void MapEditor::drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brush) {
	if(brush){
		removeTile(cellData);

		GameObject* newTile = createTile(worldPosition.x, worldPosition.y);
		cellData->addGameObject(newTile, mLayer);

		Renderer* renderer = newTile->getComponents<Renderer>()->get(0);
		Renderer* brushRenderer = brush->getComponents<Renderer>()->get(0);

		renderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y,
			brushRenderer->getRegionSize().x, brushRenderer->getRegionSize().y);
	}

}

// ---------------------------------------------------------------------------

void MapEditor::removeTile(CellData *cellData) {
	if (cellData->get(mLayer)) {
		getGameObject()->getScene()->removeGameObject(cellData->get(mLayer));
		cellData->removeGameObject(mLayer);
	}

	mSelectedTile = nullptr;
}

// ---------------------------------------------------------------------------

void MapEditor::cameraZoom() {
	f32 scroll = Input::getScroll();
	mZoom += std::fabs(scroll) * 0.05f * Time::getDeltaTimeSeconds();

	if (scroll == 1) {
		mCamera->setZoom(1.0f / mZoom);
	} else if (scroll == -1) {
		mCamera->setZoom(mZoom);
	}
}

// ---------------------------------------------------------------------------

void MapEditor::switchCameraControl() {
mCameraControl = !mCameraControl;
}

// ---------------------------------------------------------------------------

void MapEditor::processMovement() {

f32 movement = (mCameraControl ? 2000.0f * Time::getDeltaTimeSeconds() : 500.0f);

if (!mCameraControl) {
mCameraTransform->setParent(mPlayer->getTransform());
mCameraTransform->setLocalPosition(Vector3(0, 0, 0));
}

Renderer* renderer = mCameraControl ? nullptr : mPlayer ? mPlayer->getComponents<Renderer>()->get(0) : nullptr;

if (Input::isKeyPressed(GLFW_KEY_UP)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(0, movement, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,movement,0));

		if (renderer) {
			//renderer->setAnimation("run");
		}
	}

} else if (Input::isKeyPressed(GLFW_KEY_DOWN)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(0, -movement, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,-movement,0));

		if (renderer) {
			//renderer->setAnimation("run");
		}
	}
} else if (Input::isKeyPressed(GLFW_KEY_LEFT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(-movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(-movement,0,0));

		if (renderer) {
			//renderer->setAnimation("run");
			renderer->setInvertXAxis(true);
		}
	}
} else if (Input::isKeyPressed(GLFW_KEY_RIGHT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(movement,0,0));

		if (renderer) {
			//renderer->setAnimation("run");
			renderer->setInvertXAxis(false);
		}
	}
} else {
	if(!mCameraControl){
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,0,0));

		if (renderer) {
			//renderer->setAnimation("idle");
		}
	}
}
}

// ---------------------------------------------------------------------------

void MapEditor::loadMapIntoGrid() {
		// getGameObject()->getScene()->loadScene("config/scene.conf");

	const List<GameObject*>* gameObects = getGameObject()->getScene()->getNewGameObjects();

	FOR_LIST (it, gameObects){

		GameObject* gameObject = it.get();

		if (gameObject->isStatic()) {

			Transform* t = gameObject->getTransform();
			Vector3 worldPosition(t->getWorldPosition());
			Vector2 gridPosition(worldPosition.x / mGridTileSize + mGridSize / 2.0f,
					worldPosition.y / mGridTileSize + mGridSize / 2.0f);

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
			cellData->addGameObject(gameObject, gameObject->getComponents<Renderer>()->get(0)->getLayer());
		}

	}
}

// ---------------------------------------------------------------------------

void MapEditor::terminate() {

	if(mGrid){
		FOR_RANGE(i, 0, mGridSize){
			FOR_RANGE(j, 0, mGridSize){
				Memory::free<CellData>(mGrid->get(i)->get(j));
			}

			Memory::free<Array<CellData*>>(mGrid->get(i));
		}
	}

	Memory::free<Array<Array<CellData*>*>>(mGrid);

	Memory::free<ConfigMap>(mConfigMap);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
