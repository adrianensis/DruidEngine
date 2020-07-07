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

namespace DE {

const std::string MapEditor::MapEditorUI::StringsUI::smLayer = "Layer";
const std::string MapEditor::MapEditorUI::StringsUI::smTile = "Tile";
const std::string MapEditor::MapEditorUI::StringsUI::smZoom = "Zoom";
const std::string MapEditor::MapEditorUI::StringsUI::smBrush = "Brush";
const std::string MapEditor::MapEditorUI::StringsUI::smSave = "Save";
const std::string MapEditor::MapEditorUI::StringsUI::smCollider = "Collider";
const std::string MapEditor::MapEditorUI::StringsUI::smAtlas = "Atlas";
const std::string MapEditor::MapEditorUI::StringsUI::smPlay = "Play";
const std::string MapEditor::MapEditorUI::StringsUI::smInspectorTileX = "x:";
const std::string MapEditor::MapEditorUI::StringsUI::smInspectorTileY = "y:";
const std::string MapEditor::MapEditorUI::StringsUI::smInspectorTileCollider = "collider:";
const std::string MapEditor::MapEditorUI::StringsUI::smInspectorTileTag = "tag:";

// ---------------------------------------------------------------------------

MapEditor::CellData::CellData() :
		DE_Class() {

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

MapEditor::Brush::Brush() :
		DE_Class() {

}

// ---------------------------------------------------------------------------

MapEditor::Brush::~Brush() {
	free();
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::init(u32 size) {
	mGridSize = size;

	if(mGrid) free();

	mGrid = Memory::allocate<Array<GameObject*>>();
	mGrid->init(mGridSize*mGridSize);
}

// ---------------------------------------------------------------------------

GameObject* MapEditor::Brush::getTile(u32 i, u32 j){
	return mGrid->get(i * mGridSize + j);
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::addTile(GameObject* tile){
	if(mLastIndex < mGridSize * mGridSize){
		mGrid->set(mLastIndex,tile);
		mLastIndex++;

		VAR(u32, mLastIndex);
		tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));
	}
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::free(){
	Memory::free<Array<GameObject*>>(mGrid);
}

// ---------------------------------------------------------------------------

void MapEditor::Brush::clear(){
	FOR_ARRAY(i, mGrid){
		GameObject* tile = mGrid->get(i);

		if(tile)
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
	}

	mLastIndex = 0;
	mGrid->clear();
	mGrid->init(mGridSize*mGridSize);
}

// ---------------------------------------------------------------------------

MapEditor::MapEditorUI::MapEditorUI(){

}

MapEditor::MapEditorUI::~MapEditorUI(){
	Memory::free<Array<UIButton*>>(mAtlasButtons);
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::createMenuBarUI() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 1;
	f32 baseX = -1.3f;
	f32 baseY = 0.9f;
	f32 sizeChar = 0.045f;
	f32 separatorSize = 0.01f;

	std::string str = StringsUI::smSave;
	f32 sizeButtonAccumulated = 0;
	f32 sizeLastButton = 0;
	f32 sizeButton = sizeChar * str.length();
	UIButton* button = UI::getInstance()->createButton(scene, Vector2(baseX, baseY), Vector2(sizeButton, 0.1f), mUILayer);
	button->setLabel(str);
	i++;
	button->setOnPressedCallback([&, self = button]() {
		self->getScene()->saveScene(self->getScene()->getPath());
	});
	sizeButtonAccumulated = sizeButton/2.0f;

	str = StringsUI::smCollider;
	sizeButton = sizeChar * str.length();
	button = UI::getInstance()->createButton(scene, Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), mUILayer);
	button->setLabel(str);
	i++;
	button->setOnPressedCallback([&, self = button]() {
		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());

	});
	sizeButtonAccumulated += sizeButton + separatorSize;

	str = StringsUI::smAtlas;
	sizeButton = sizeChar * str.length();
	button = UI::getInstance()->createButton(scene, Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), mUILayer);
	button->setLabel(str);
	i++;
	button->setOnPressedCallback([&, self = button, mapEditorUI = this]() {
		mapEditorUI->toggleAtlasUI();
	});
	sizeButtonAccumulated += sizeButton + separatorSize;


	str = StringsUI::smPlay;
	sizeButton = sizeChar * str.length();
	button = UI::getInstance()->createButton(scene, Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), mUILayer);
	button->setLabel(str);
	i++;
	button->setOnPressedCallback([&, self = button]() {
		if (! mMapEditor->mPlayer) {
			 mMapEditor->createPlayer();
		}

		 mMapEditor->switchCameraControl();
	});
	sizeButtonAccumulated += sizeButton + separatorSize;
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::createInspectorUI() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 0;
	f32 baseX = 0.85f;
	f32 baseY = 0.9f;
	f32 separatorSize = 0.05f;
	f32 size = 0.2f;

	mTextInspectorTag = UI::getInstance()->createText(scene, Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileTag, mUILayer);
	i++;
	mTextInspectorX = UI::getInstance()->createText(scene, Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileX + "0.000000", mUILayer);
	i++;
	mTextInspectorY = UI::getInstance()->createText(scene, Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileY + "0.000000", mUILayer);
	i++;
	mTextInspectorCollider = UI::getInstance()->createText(scene, Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileCollider, mUILayer);
	//UIButton* button;
	f32 buttonSize = 0.15f;
	mButtonInspectorCollider = UI::getInstance()->createButton(scene, Vector2(1.3f, baseY - separatorSize * i), Vector2(buttonSize, 0.05f), mUILayer);
	mButtonInspectorCollider->setLabel("[ ]");
	mButtonInspectorCollider->setOnPressedCallback([&, self = mButtonInspectorCollider, mapEditor = mMapEditor]() {
		if(mapEditor->mTile){
			List<Collider*>* colliders = mapEditor->mTile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				mapEditor->mTile->removeComponent<Collider>(colliders->get(0));
				mapEditor->mTile->removeComponent<RigidBody>(mapEditor->mTile->getComponents<RigidBody>()->get(0));
				self->setLabel("[ ]");
			} else {
				mapEditor->addColliderToTile(mapEditor->mTile);
				self->setLabel("[X]");
			}
		}
	});

	i++;
//
//	button = UI::getInstance()->createButton(scene, Vector2(x, 0.75f), Vector2(size*i, 0.1f), mUILayer);
//	button->setLabel("Collider");
//	i++;
//	button->setOnPressedCallback([&, self = button]() {
//		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());
//	});
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::createAtlasUI() {

	f32 tileSize = 0.07f;

	Vector2 screenOffset = Vector2(-1.0f, 0.0f);

	Vector2 atlasSize = Vector2(8, 16);
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	mAtlasButtons = Memory::allocate<Array<UIButton*>>();
	mAtlasButtons->init(atlasSize.x*atlasSize.y);

	FOR_RANGE(i, 0, atlasSize.x)
	{
		FOR_RANGE(j, 0, atlasSize.y)
		{
			UIButton* tile = UI::getInstance()->createButton(mMapEditor->getGameObject()->getScene(),
					Vector2((i - (atlasSize.x / 2.0f)) * tileSize + screenOffset.x,
							((atlasSize.y / 2.0f) - j) * tileSize - screenOffset.y), Vector2(tileSize, tileSize), mUILayer);

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(mMapEditor->mMaterial);
			renderer->setRegion(i / atlasSize.x, j / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y);
			//renderer->setLayer(2);

			tile->setOnPressedCallback([&, self = tile, mapEditor = mMapEditor]() {
				Renderer* buttonRenderer = self->getRenderer();
				mapEditor->mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x,
						buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
						buttonRenderer->getRegionSize().y);

				if((mapEditor->mBrush.mLastIndex < (mapEditor->mBrush.mGridSize * mapEditor->mBrush.mGridSize)) &&
						Input::isModifierPressed(GLFW_MOD_CONTROL)){
					mapEditor->mBrush.addTile(self);
				} else {
					mapEditor->mBrush.clear();
					mapEditor->mBrush.addTile(self);
				}

				mapEditor->mIsPaintMode = true;
			});

			mAtlasButtons->set(i*atlasSize.y + j, tile);
		}
	}

	f32 atlasBackgroundMargin = 0.1f;

	mAtlasBackground = Memory::allocate<GameObject>();
	mAtlasBackground->init();

	mAtlasBackground->setShouldPersist(false);

	mAtlasBackground->getTransform()->setLocalPosition(Vector3((-tileSize/2.0f + screenOffset.x) / RenderContext::getAspectRatio(), tileSize/2.0f + screenOffset.y, 0));
	mAtlasBackground->getTransform()->setScale(
			Vector3((atlasSize.x * tileSize + atlasBackgroundMargin) / RenderContext::getAspectRatio(),
					atlasSize.y * tileSize + atlasBackgroundMargin, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mAtlasBackground->addComponent<Renderer>(renderer);

	renderer->setLayer(mUILayer);
	renderer->setAffectedByProjection(false);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadMaterial("resources/button.bmp"));
	//renderer->setColor(Vector4(0,0,0,1));

	mMapEditor->getGameObject()->getScene()->addGameObject(mAtlasBackground);

	toggleAtlasUI();
}


// ---------------------------------------------------------------------------


void MapEditor::MapEditorUI::createUI() {

	Scene* scene = mMapEditor->getGameObject()->getScene();
	mTextBrush = UI::getInstance()->createText(scene, Vector2(-1.3f, -0.8f), mTextSize, StringsUI::smBrush + " 0",
			mUILayer);
	mTextLayer = UI::getInstance()->createText(scene, Vector2(-1.3f, -0.85f), mTextSize, StringsUI::smLayer + " 0",
			mUILayer);
	mTextTile = UI::getInstance()->createText(scene, Vector2(-1.3f, -0.9f), mTextSize, StringsUI::smTile, mUILayer);

	// mButtons = Memory::allocate<List<UIButton*>>();
	// mButtons->init();

	createMenuBarUI();
	createInspectorUI();
	createAtlasUI();
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::updateUI() {
	mTextBrush->setText(StringsUI::smBrush + " " + std::to_string(mMapEditor->mBrush.mGridSize));
	mTextLayer->setText(StringsUI::smLayer + " " + std::to_string(mMapEditor->mLayer));

	/*Renderer* brushRenderer = mMapEditor->mBrush->getComponents<Renderer>()->get(0);

	std::string strTile = StringsUI::mTile;
	strTile += " ";
	strTile += std::to_string(brushRenderer->getRegionPosition().x).substr(0, 6);
	strTile += ",";
	strTile += std::to_string(brushRenderer->getRegionPosition().y).substr(0, 6);

	mTextTile->setText(strTile);*/

	updateInspectorUI();
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::updateInspectorUI() {

	if(mMapEditor->mTile){
		mTextInspectorTag->setText(StringsUI::smInspectorTileTag + mMapEditor->mTile->getTag());
		mTextInspectorX->setText(StringsUI::smInspectorTileX + std::to_string(mMapEditor->mTile->getTransform()->getLocalPosition().x));
		mTextInspectorY->setText(StringsUI::smInspectorTileY + std::to_string(mMapEditor->mTile->getTransform()->getLocalPosition().y));

		List<Collider*>* colliders = mMapEditor->mTile->getComponents<Collider>();
		bool hasCollider = colliders && !colliders->isEmpty();

		mButtonInspectorCollider->setLabel(hasCollider ? "[X]" : "[ ]");
	}
}

// ---------------------------------------------------------------------------

void MapEditor::MapEditorUI::toggleAtlasUI(){
	mIsAtlasShow = !mIsAtlasShow;

	FOR_ARRAY(i, mAtlasButtons){
		mAtlasButtons->get(i)->getRenderer()->setIsActive(mIsAtlasShow);
		mAtlasButtons->get(i)->getCollider()->setIsActive(mIsAtlasShow);
	}

	mAtlasBackground->getComponents<Renderer>()->get(0)->setIsActive(mIsAtlasShow);
}

// ---------------------------------------------------------------------------

MapEditor::MapEditor() :
		Script() {

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

// ---------------------------------------------------------------------------

void MapEditor::resetBrush() {
	mIsPaintMode = false;
	mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(0, 0, 1, 1);
}

// ---------------------------------------------------------------------------

void MapEditor::createBrush() {
	Vector2 size(mTileSize / 2.0f, mTileSize / 2.0f);

	mBrushCursor = Memory::allocate<GameObject>();
	mBrushCursor->init();

	mBrushCursor->getTransform()->setLocalPosition(Vector3(0, 0, 0));
	mBrushCursor->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mBrushCursor->addComponent<Renderer>(renderer);

	renderer->setLayer(mMapEditorUI.mUILayer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mMaterial);

	getGameObject()->getScene()->addGameObject(mBrushCursor);

	mBrush.init(1);
}

// ---------------------------------------------------------------------------

GameObject* MapEditor::createTile(f32 x, f32 y) {
	// if(mBrush){
	Vector2 size(mTileSize, mTileSize);

	GameObject* tile = Memory::allocate<GameObject>();
	tile->init();

	tile->getTransform()->setLocalPosition(Vector3(x, y, 0));
	tile->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	tile->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mMaterial);

	renderer->setLayer(mLayer);

	// renderer->setLineMode(true);

//	RigidBody* rigidBody = Memory::allocate<RigidBody>();
//	tile->addComponent<RigidBody>(rigidBody);
//
//	Collider* collider = Memory::allocate<Collider>();
//	tile->addComponent<Collider>(collider);
//	collider->setSize(size.x,size.y);

	tile->setIsStatic(true);
	tile->setShouldPersist(true);

	getGameObject()->getScene()->addGameObject(tile);
	// }

	return tile;
}


// ---------------------------------------------------------------------------

void MapEditor::init() {
	mTransform = getGameObject()->getTransform();
	mPlayer = nullptr;
	mTestCreated = false;
	mMaterial = nullptr;

	mBrushCursor = nullptr;
	mBrushSize = 0;

	mLayer = 0;

	mCamera = nullptr;
	mCameraTransform = nullptr;

	mTileIndex = 0;

	mGridSize = 50;
	mTileSize = 100;

	mTile = nullptr;

	mZoom = 1;

	mCameraControl = true;

	mIsPaintMode = false;
}

// ---------------------------------------------------------------------------

void MapEditor::firstStep() {

	mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
	mCameraTransform = mCamera->getGameObject()->getTransform();

	mMaterial = MaterialManager::getInstance()->loadMaterial("resources/tiles.png");

	mGrid = Memory::allocate<Array<Array<CellData*>*>>();
	mGrid->init(mGridSize);

	f32 halfGridSize = mGridSize / 2.0f;

	FOR_RANGE(i, 0, mGridSize)
	{
		mGrid->set(i, Memory::allocate<Array<CellData*>>());
		mGrid->get(i)->init(mGridSize);
		FOR_RANGE(j, 0, mGridSize)
		{
			CellData* cellData = Memory::allocate<CellData>();
			mGrid->get(i)->set(j, cellData);
		}
	}

	loadMapIntoGrid();

	createBrush();

	mMapEditorUI.init(this);

	mMapEditorUI.createUI();
}

// ---------------------------------------------------------------------------

void MapEditor::step() {

	Vector2 mouse(Input::getMousePosition());
	Vector3 world = mCamera->screenToWorld(mouse);

	Vector3 clampedPosition(std::roundf(world.x / mTileSize) * mTileSize, std::roundf(world.y / mTileSize) * mTileSize,
			0);

	//click(clampedPosition);

	FOR_RANGE(i, 0, mBrush.mGridSize){
		FOR_RANGE(j, 0, mBrush.mGridSize){
			f32 offset = mTileSize;
			click(clampedPosition + Vector3(offset*j, -offset*i, 0), /*mBrushOLD*/ mBrush.getTile(i, j));
		}
	}

	if (mBrushCursor)
		mBrushCursor->getTransform()->setLocalPosition(world);

	cameraZoom();
	processMovement();

	if (Input::isKeyPressedOnce(GLFW_KEY_KP_ADD)) {
		if (mBrush.mGridSize < 2){
			mBrush.mGridSize++;
			mBrush.init(mBrush.mGridSize);
		}
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_KP_SUBTRACT)) {
		if (mBrush.mGridSize > 1){
			mBrush.mGridSize--;
			mBrush.init(mBrush.mGridSize);
		}
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_Z)) {
		if (mLayer < 10)
			mLayer++;
		VAR(u32, mLayer)
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_X)) {
		if (mLayer > 0)
			mLayer--;
		VAR(u32, mLayer)
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_ESCAPE)) {
		resetBrush();
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_TAB)) {
		mMapEditorUI.toggleAtlasUI();
	}

	if (Input::isKeyPressedOnce(GLFW_KEY_S) && Input::isModifierPressed(GLFW_MOD_CONTROL /*| GLFW_MOD_SHIFT*/)) {
		getGameObject()->getScene()->saveScene(getGameObject()->getScene()->getPath());
	}

	mMapEditorUI.updateUI();
}

// ---------------------------------------------------------------------------

void MapEditor::click(const Vector3 &clampedPosition, GameObject* brush) {

	f32 halfSize = (mGridSize * mTileSize / 2.0f);
	if (std::fabs(clampedPosition.x) < halfSize && std::fabs(clampedPosition.y) < halfSize) {

		Vector2 gridPosition(clampedPosition.x / mTileSize + mGridSize / 2.0f,
				clampedPosition.y / mTileSize + mGridSize / 2.0f);

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
		if(mTile){
			mTile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
		}

		mTile = cellData->get(mLayer);
		mTile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));
	}
}

// ---------------------------------------------------------------------------

void MapEditor::drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brush) {
	if(brush){
		if (!cellData->get(mLayer)) {
			cellData->addGameObject(createTile(worldPosition.x, worldPosition.y), mLayer);
		}

		Renderer* renderer = cellData->get(mLayer)->getComponents<Renderer>()->get(0);
		Renderer* brushRenderer = brush->getComponents<Renderer>()->get(0);

		renderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y,
			brushRenderer->getRegionSize().x, brushRenderer->getRegionSize().y);
				// renderer->setRegion(0, 0, 1, 1);
	}

}

// ---------------------------------------------------------------------------

void MapEditor::removeTile(CellData *cellData) {
	if (cellData->get(mLayer)) {
		getGameObject()->getScene()->removeGameObject(cellData->get(mLayer));
		cellData->removeGameObject(mLayer);
	}
}

// ---------------------------------------------------------------------------

void MapEditor::cameraZoom() {
f32 scroll = Input::getScroll();
mZoom += std::fabs(scroll) * 0.5f * Time::getDeltaTimeSeconds();

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
			renderer->setAnimation("run");
		}
	}

} else if (Input::isKeyPressed(GLFW_KEY_DOWN)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(0, -movement, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,-movement,0));

		if (renderer) {
			renderer->setAnimation("run");
		}
	}
} else if (Input::isKeyPressed(GLFW_KEY_LEFT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(-movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(-movement,0,0));

		if (renderer) {
			renderer->setAnimation("run");
			renderer->setInvertXAxis(true);
		}
	}
} else if (Input::isKeyPressed(GLFW_KEY_RIGHT)) {
	if(mCameraControl){
		mCameraTransform->translate(Vector3(movement, 0, 0));
	} else {
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(movement,0,0));

		if (renderer) {
			renderer->setAnimation("run");
			renderer->setInvertXAxis(false);
		}
	}
} else {
	if(!mCameraControl){
		mPlayer->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0,0,0));

		if (renderer) {
			renderer->setAnimation("idle");
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
			Vector2 gridPosition(worldPosition.x / mTileSize + mGridSize / 2.0f,
					worldPosition.y / mTileSize + mGridSize / 2.0f);

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
			cellData->addGameObject(gameObject, gameObject->getComponents<Renderer>()->get(0)->getLayer());
		}

	}
}

// ---------------------------------------------------------------------------

void MapEditor::terminate() {

	FOR_RANGE(i, 0, mGridSize){
		FOR_RANGE(j, 0, mGridSize){
			Memory::free<CellData>(mGrid->get(i)->get(j));
		}

		Memory::free<Array<CellData*>>(mGrid->get(i));
	}

	Memory::free<Array<Array<CellData*>*>>(mGrid);

			// Memory::free<List<UIButton*>>(mButtons);

	//Memory::free<MapEditorUI>(mInfoBarUI);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
