#include "MapEditorUI.hpp"

#include <TimeUtils.hpp>

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

#include "TimerManager.hpp"

#include "Grid.hpp"

#include "GameObjectBuilder.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Brush::Brush() : DE_Class() {

}

// ---------------------------------------------------------------------------

Brush::~Brush() {
	free();
}

// ---------------------------------------------------------------------------

void Brush::init(MapEditor* mapEditor) {

	mMapEditor = mapEditor;

	if(mGrid) free();

	mGrid = Memory::allocate<Array<GameObject*>>();
	mGrid->init(mBrushMaxGridSize);

	//Vector2 size(mMapEditor->mMapEditorUI.mGridTileSize / 2.0f, mMapEditor->mMapEditorUI.mGridTileSize / 2.0f);

	if(mBrushCursor) {
		mMapEditor->getGameObject()->getScene()->removeGameObject(mBrushCursor);
	}

	mBrushCursor = GameObjectBuilder::getInstance()->
	createSprite(mMapEditor->mMaterial, mMapEditor->mMapEditorUI.mUILayer, true)->getGameObject();

	mBrushCursor->getTransform()->setLocalPosition(Vector3(-1.4f / RenderContext::getAspectRatio(), 0.8f, 0));
	mBrushCursor->getTransform()->setScale(Vector3(0.1f / RenderContext::getAspectRatio(), 0.1f, 1));
	mBrushCursor->getTransform()->setAffectedByProjection(false);

	mMapEditor->getGameObject()->getScene()->addGameObject(mBrushCursor);
}

// ---------------------------------------------------------------------------

GameObject* Brush::getTile(u32 i, u32 j){
	return mGrid->get(i * mBrushGridSize.x + j);
}

// ---------------------------------------------------------------------------

void Brush::addTile(GameObject* tile, Vector2 atlasPosition){
	if(mLastIndex < mBrushMaxGridSize){

		// If it's the first selected tile, take it as coordinates origin.
		if(! mGrid->get(0)){
			mOriginAtlasPosition = atlasPosition;
		}

		Vector2 distance(std::abs(atlasPosition.x - mOriginAtlasPosition.x),
				std::abs(atlasPosition.y - mOriginAtlasPosition.y));

		mBrushGridSize.x = std::max(mBrushGridSize.x,distance.x + 1);
		mBrushGridSize.y = std::max(mBrushGridSize.y,distance.y + 1);

		mGrid->set(mLastIndex,tile);
		mLastIndex++;
		tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));
	}
}

// ---------------------------------------------------------------------------

void Brush::free(){
	mMapEditor->getGameObject()->getScene()->removeGameObject(mBrushCursor);
	Memory::free<Array<GameObject*>>(mGrid);
}

// ---------------------------------------------------------------------------

void Brush::clear(){
	FOR_RANGE(i, 0, mLastIndex){
		GameObject* tile = mGrid->get(i);

		if(tile)
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
	}

	mLastIndex = 0;
	mBrushGridSize = Vector2(1,1);
	mOriginAtlasPosition = Vector2(0,0);
	mGrid->clear();
}

// ---------------------------------------------------------------------------

void Brush::setDrawTileSize(f32 size) {
	mDrawTileSize = size;
	//mBrushCursor->getTransform()->setScale(Vector3(mDrawTileSize,mDrawTileSize,1));
}

// ---------------------------------------------------------------------------

void MapEditorUI::resetBrush() {
	mMapEditor->mGrid.mIsPaintMode = false;
	mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(0, 0, 1, 1);
	mBrush.clear();
}

// ---------------------------------------------------------------------------

void MapEditorUI::createBrush() {
	mBrush.init(mMapEditor);
	mBrush.setDrawTileSize(mMapEditor->mGrid.getGridTileSize());
}


// ---------------------------------------------------------------------------

MapEditorUI::MapEditorUI(){

}

MapEditorUI::~MapEditorUI(){
	Memory::free<Array<UIButton*>>(mLayerButtons);
}

// ---------------------------------------------------------------------------

void MapEditorUI::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	UI::getInstance()->getBuilder()->setScene(mMapEditor->getGameObject()->getScene()); // TODO: Move this to a more "Core" place

	createMenuBar();
	createInspector();
	createAtlasSelector();
	createSprites();
	createLayersBar();

	toggleAtlas();

	createBrush();

	DE_SUBSCRIBE_TO_EVENT(MapEditorOnSelectTile, [this](const Event* event){
		updateInspectorOnSelectTile();
	});
}

// ---------------------------------------------------------------------------

void MapEditorUI::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 1;
	f32 baseX = -1.4f;
	f32 baseY = 0.92f;

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mUILayer);

	UI::getInstance()->getBuilder()->
	setText(mStringsUI.smSave)->
	create(UIElementType::BUTTON)->
	getUIElement()->
	setOnPressedCallback([&]() {
		scene->saveScene(scene->getPath());
	});

	UI::getInstance()->getBuilder()->
	setText(mStringsUI.smCollider)->
	create(UIElementType::BUTTON)->
	getUIElement()->
	setOnPressedCallback([&]() {
		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());

	});

	UI::getInstance()->getBuilder()->
	setText(mStringsUI.smAtlas)->
	create(UIElementType::BUTTON)->
	getUIElement()->
	setOnPressedCallback([&, mapEditorUI = this]() {
		mapEditorUI->toggleAtlas();
	});

	UI::getInstance()->getBuilder()->
	setText(mStringsUI.smPlay)->
	create(UIElementType::BUTTON)->
	getUIElement()->
	setOnPressedCallback([&]() {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			 mMapEditor->createPlayer();
		}

		 mMapEditor->switchCameraControl();
	});

	UI::getInstance()->getBuilder()->
	setText(mStringsUI.smGrid)->
	create(UIElementType::BUTTON)->
	getUIElement()->
		setOnPressedCallback([&, mapEditorUI = this]() {
		mapEditorUI->toggleGrid();

	});

	/*button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * mStringsUI.smCreateSprite.length(), 0.1f))->
		setText(mStringsUI.smCreateSprite)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, mapEditorUI = this]() {
		//mapEditorUI->toggleGrid();

	});*/

	/*UIDropdown* uiDropdown = (UIDropdown*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * 8, 0.1f))->
		setText("Dropdown")->
		create(UIElementType::DROPDOWN)->
		getUIElement();

	uiDropdown->addOption("Option 1");
	uiDropdown->addOption("Option 2");
	uiDropdown->addOption("Option 3");*/

}

// ---------------------------------------------------------------------------

void MapEditorUI::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 0;
	f32 baseX = 0.84f;
	f32 baseY = 0.92f;
	f32 separatorSize = 0.052f;

	Vector2 panelSize(0.5f, 1.6f);
	Vector2 panelOffset(-0.1f,0.04f);

	UIElement* uiPanel = (UIElement*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX + panelSize.x/2.0f + panelOffset.x, baseY - panelSize.y/2.0f + panelOffset.y))->
		setLayer(mUILayer-1)->
		setAdjustSizeToText(false)->
		setSize(panelSize)->
		create(UIElementType::PANEL)->
		getUIElement();

	mTextInspectorTag = (UIText*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(mTextSize)->
		setText(mStringsUI.smInspectorTileTag)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxTag = (UIText*) UI::getInstance()->getBuilder()->
		setText("")->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxTag->setOnTextChangedCallback([self = mTextBoxTag, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			std::string tag(self->getInputString());
			tile->setTag(tag);
		});
	});

	mTextInspectorX = (UIText*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.smInspectorTileX + "0.000000")->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextInspectorY = (UIText*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.smInspectorTileY + "0.000000")->
		create(UIElementType::TEXT)->
		getUIElement();


	mTextInspectorY = (UIText*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.smInspectorTileCollider)->
		create(UIElementType::TEXT)->
		getUIElement();

	mButtonInspectorCollider = (UIButton*) UI::getInstance()->getBuilder()->
			setSize(Vector2(0.15f, 0.05f))->
			setText("asdasd")->
			create(UIElementType::BUTTON)->
			getUIElement();

	mButtonInspectorCollider->setOnPressedCallback([&, self = mButtonInspectorCollider, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			List<Collider*>* colliders = tile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				tile->removeComponent<Collider>(colliders->get(0));
				tile->removeComponent<RigidBody>(tile->getComponents<RigidBody>()->get(0));
				self->setText("[ ]");
			} else {
				mapEditor->addColliderToTile(tile);
				self->setText("[X]");
			}
		});
	});

	mTextTileSize = (UIText*) UI::getInstance()->getBuilder()->
		setSize(mTextSize)->
		setText(mStringsUI.smInspectorSize)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxSizeX = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxSizeX->setOnTextChangedCallback([self = mTextBoxSizeX, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([self = self, mapEditor = mapEditor](GameObject* tile){
			std::string width(self->getInputString());

			Transform* tileTransform = tile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(std::stof(width.length() > 0 ? width : "0"), scale.y, scale.z));
			tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
		});
	});

	mTextBoxSizeY = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxSizeY->setOnTextChangedCallback([self = mTextBoxSizeY, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([self = self, mapEditor = mapEditor](GameObject* tile){
			std::string height(self->getInputString());

			Transform* tileTransform = tile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(scale.x, std::stof(height.length() > 0 ? height : "0"), scale.z));
			tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
		});
	});

	mTextColliderPos = (UIText*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.smInspectorPosCollider)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxColliderPosX = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxColliderPosX->setOnTextChangedCallback([self = mTextBoxColliderPosX, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			std::string x(self->getInputString());

			auto colliderList = tile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider){
				Vector3 offset = collider->getPositionOffset();
				offset.x = std::stof(x.length() > 0 ? x : "0");
				collider->setPositionOffset(offset);
			}
		});
	});

	mTextBoxColliderPosY = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxColliderPosY->setOnTextChangedCallback([self = mTextBoxColliderPosY, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			std::string y(self->getInputString());

			auto colliderList = tile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider){
				Vector3 offset = collider->getPositionOffset();
				offset.y = std::stof(y.length() > 0 ? y : "0");
				collider->setPositionOffset(offset);
			}
		});
	});

	mTextColliderSize = (UIText*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.smInspectorSizeCollider)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxColliderSizeX = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxColliderSizeX->setOnTextChangedCallback([self = mTextBoxColliderSizeX, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			std::string width(self->getInputString());

			auto colliderList = tile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider)
				collider->setSize(std::stof(width.length() > 0 ? width : "0"), collider->getHeight());
		});
	});

	mTextBoxColliderSizeY = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	mTextBoxColliderSizeY->setOnTextChangedCallback([self = mTextBoxColliderSizeY, mapEditor = mMapEditor]() {
		mapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			std::string height(self->getInputString());

			auto colliderList = tile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider)
				collider->setSize(collider->getWidth(), std::stof(height.length() > 0 ? height : "0"));
		});
	});

}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspector() {

	if(mMapEditor->mGrid.getFirstSelectedTile()){
		Transform* tileTransform = mMapEditor->mGrid.getFirstSelectedTile()->getTransform();

		mTextInspectorX->setText(mStringsUI.smInspectorTileX + std::to_string(tileTransform->getLocalPosition().x));
		mTextInspectorY->setText(mStringsUI.smInspectorTileY + std::to_string(tileTransform->getLocalPosition().y));

		List<Collider*>* colliders = mMapEditor->mGrid.getFirstSelectedTile()->getComponents<Collider>();
		bool hasCollider = colliders && !colliders->isEmpty();

		mButtonInspectorCollider->setText(hasCollider ? "[X]" : "[ ]");
	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspectorOnSelectTile() {

	if(mMapEditor->mGrid.getFirstSelectedTile()){
		updateInspector();

		std::string tag = mMapEditor->mGrid.getFirstSelectedTile()->getTag();
		mTextBoxTag->setText(/*tag.length() > 0 ?*/ tag /*: "none"*/);

		Transform* tileTransform = mMapEditor->mGrid.getFirstSelectedTile()->getTransform();

		mTextBoxSizeX->setText(std::to_string(tileTransform->getScale().x));
		mTextBoxSizeY->setText(std::to_string(tileTransform->getScale().y));

		auto colliderList = mMapEditor->mGrid.getFirstSelectedTile()->getComponents<Collider>();
		Collider* collider = colliderList && !colliderList->isEmpty() ? colliderList->get(0) : nullptr;

		mTextBoxColliderSizeX->setText(std::to_string(collider ? collider->getWidth() : 0.0f));
		mTextBoxColliderSizeY->setText(std::to_string(collider ? collider->getHeight() : 0.0f));

	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::createAtlas(u32 index, Material* material) {

	f32 tileSize = 0.07f;

	Vector2 screenOffset = Vector2(0.0f, 0.0f);

	Vector2 atlasSize = Vector2(mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.width"), mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.height"));
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	UIElementData data;

	UI::getInstance()->getBuilder()->
		setSeparatorSize(0)->
		setText("");

	FOR_RANGE(i, 0, atlasSize.x){

		UI::getInstance()->getBuilder()->
			setLayout(UILayout::VERTICAL)->
			setPosition(Vector2((i - (atlasSize.x / 2.0f)) * tileSize + screenOffset.x,
				((atlasSize.y / 2.0f)) * tileSize - screenOffset.y))->
				setSize(Vector2(tileSize, tileSize))->
				setBackgroundColor(Vector4(0,0,0,1))->
				setLayer(mUILayer);

		FOR_RANGE(j, 0, atlasSize.y){

			UIButton* tile = (UIButton*) UI::getInstance()->getBuilder()->
					create(UIElementType::BUTTON)->
					getUIElement();

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(material);
			renderer->setRegion(i / atlasSize.x, j / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y);

			tile->setOnPressedCallback([&, self = tile, mapEditor = mMapEditor, i = i, j = j]() {
				Renderer* buttonRenderer = self->getRenderer();
				mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x,
						buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
						buttonRenderer->getRegionSize().y);

				Vector2 atlasPosition = Vector2(i, j);

				if((mBrush.mLastIndex < mBrush.mBrushMaxGridSize) &&
						Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL)){
					mBrush.addTile(self, atlasPosition);
				} else {
					mBrush.clear();
					mBrush.addTile(self, atlasPosition);
				}

				mBrush.setDrawTileSize(mMapEditor->mGrid.getGridTileSize());
				mapEditor->mGrid.mIsPaintMode = true;
			});

			UI::getInstance()->addToGroup(mAtlasUIGroup, tile);

			//mAtlasButtons->set(i*atlasSize.y + j, tile);
		}
	}

	UI::getInstance()->getBuilder()->restoreSeparatorSize();

	// Background
	/*f32 atlasBackgroundMargin = 0.1f;

	mAtlasBackground = Memory::allocate<GameObject>();
	mAtlasBackground->init();

	mAtlasBackground->setShouldPersist(false);

	mAtlasBackground->getTransform()->setLocalPosition(Vector3((-tileSize/2.0f + screenOffset.x) / RenderContext::getAspectRatio(), tileSize/2.0f + screenOffset.y, 0));
	mAtlasBackground->getTransform()->setScale(
			Vector3((atlasSize.x * tileSize + atlasBackgroundMargin) / RenderContext::getAspectRatio(),
					atlasSize.y * tileSize + atlasBackgroundMargin, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	mAtlasBackground->addComponent<Renderer>(renderer);

	renderer->setLayer(mUILayer-1);
	renderer->setAffectedByProjection(false);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadMaterial("resources/button.png"));

	mMapEditor->getGameObject()->getScene()->addGameObject(mAtlasBackground);

	UI::getInstance()->addToGroup(mAtlasUIGroup, mAtlasBackground);*/
}

void MapEditorUI::createAtlasSelector() {
	u32 maxLayers = RenderEngine::getInstance()->getMaxLayers();

	Scene* scene = mMapEditor->getGameObject()->getScene();
	UIButton* button = nullptr;

	f32 size = 0.15f;
	f32 baseX = -1.4f;
	f32 baseY = 0.65f;

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(Vector2(size,size))->
		setBackgroundColor(Vector4(0,0,0,1))->
		setText("")->
		setLayer(mUILayer);

	FOR_RANGE(i, 0, mMapEditor->mConfigMap->getU32("atlases.length")){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		create(UIElementType::BUTTON)->
		getUIElement();


		Material* material = MaterialManager::getInstance()->loadMaterial(mMapEditor->mConfigMap->getString("atlases["+ std::to_string(i) +"].texture"));
		button->getRenderer()->setMaterial(material);

		button->setOnPressedCallback([&, i=i, material=material, this]() {
			mMapEditor->mMaterial = material;
			createBrush();
			UI::getInstance()->removeElementsFromGroup(mAtlasUIGroup);
			TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [this,i,material](){
				createAtlas(i, material);
			});
		});

		UI::getInstance()->addToGroup(mAtlasSelectorUIGroup, button);
	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::update() {
	updateInspector();
	updateGridLines();
}

// ---------------------------------------------------------------------------

void MapEditorUI::updateGridLines() {
	if(mIsGridShow){
		f32 halfLinesCount = 30;
		f32 lineLength = 5000;

		// GRID LINES
		FOR_RANGE(i, -halfLinesCount, halfLinesCount){

			f32 pos = (i * mMapEditor->mGrid.getGridTileSize()) + mMapEditor->mGrid.getGridTileSize()/2.0f;

			RenderEngine::getInstance()->drawLine(Vector3(-lineLength,pos,0), Vector3(lineLength,pos,0), 1, true);
			RenderEngine::getInstance()->drawLine(Vector3(pos,-lineLength,0), Vector3(pos,lineLength,0), 1, true);
		}
	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::toggleGrid() {
	mIsGridShow = !mIsGridShow;
}

// ---------------------------------------------------------------------------

void MapEditorUI::toggleAtlas(){
	mIsAtlasShow = !mIsAtlasShow;

	UI::getInstance()->setGroupVisibility(mAtlasUIGroup, mIsAtlasShow);
	UI::getInstance()->setGroupVisibility(mAtlasSelectorUIGroup, mIsAtlasShow);
	UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsAtlasShow);
}

// ---------------------------------------------------------------------------

void MapEditorUI::createSprites() {

	/*
		TODO : Read sprites from config file
	*/
	/*f32 tileSize = 0.2f;
	Vector2 screenOffset = Vector2(-1.0f, 0.0f);
	f32 separator = 0.01f;
	Vector2 origin = Vector2(-1.2f, -0.7);

	Vector2 atlasSize = Vector2(4, 8);
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	f32 j = 0.5f;

	u32 spritesCount = 4;

	UIElementData data;

	UIButton* tile = (UIButton*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(origin)->
		setSize(Vector2(tileSize, tileSize))->
		setBackgroundColor(Vector4(0,0,0,1))->
		setLayer(mUILayer);

	FOR_RANGE(i, 0, spritesCount){

		UIButton* sprite = (UIButton*) UI::getInstance()->getBuilder()->
				create(UIElementType::BUTTON)->
				getUIElement();

		Renderer* renderer = sprite->getRenderer();
		renderer->setMaterial(mMapEditor->mMaterial);
		renderer->setRegion(i / atlasSize.x, j / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y);

		sprite->setOnPressedCallback([&, self = sprite, mapEditor = mMapEditor]() {
			Renderer* buttonRenderer = self->getRenderer();
			mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x,
					buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
					buttonRenderer->getRegionSize().y);

			Vector2 atlasPosition = Vector2(0, 0);

			mBrush.clear();
			mBrush.addTile(self, atlasPosition);

			mBrush.setDrawTileSize(200); // TODO : get this value from a config.

			mapEditor->mGrid.mIsPaintMode = true;
		});

		UI::getInstance()->addToGroup(mSpritesUIGroup, sprite);

		//mSpriteButtons->set(i, sprite);
	}*/
}

// ---------------------------------------------------------------------------

void MapEditorUI::createLayersBar() {

	u32 maxLayers = RenderEngine::getInstance()->getMaxLayers();
	mLayerButtons = Memory::allocate<Array<UIButton*>>();
	mLayerButtons->init(maxLayers);

	Scene* scene = mMapEditor->getGameObject()->getScene();
	UIButton* button = nullptr;

	f32 size = 0.1f;
	f32 baseX = 0.84f + size + size/2.0f;
	f32 baseY = 0.0f;

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX - size - 0.01f, baseY))->
		setSize(Vector2(size,size/2.0f))->
		restoreColors()->
		setLayer(mUILayer);

	FOR_RANGE(i, 0, maxLayers){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText(std::to_string(i))->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, mapEditorUI = this]() {
			mapEditorUI->selectLayer(i);
		});

		mLayerButtons->set(i, button);
	}

	UI::getInstance()->getBuilder()->
			setLayout(UILayout::VERTICAL)->
			setPosition(Vector2(baseX, baseY))->
			setSize(Vector2(size,size/2.0f));

	FOR_RANGE(i, 0, maxLayers){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText("on")->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, self = button]() {
			bool visibility = RenderEngine::getInstance()->getLayersData()->get(i)->mVisible;
			RenderEngine::getInstance()->getLayersData()->get(i)->mVisible = !visibility;
			self->setText(RenderEngine::getInstance()->getLayersData()->get(i)->mVisible ? "on" : "off");
		});
	}

	selectLayer(0);
}

// ---------------------------------------------------------------------------

void MapEditorUI::selectLayer(u32 layer){
	u32 maxLayers = RenderEngine::getInstance()->getMaxLayers();

	FOR_ARRAY(i, mLayerButtons){
		mLayerButtons->get(i)->getRenderer()->setColor(Vector4(0.6f,0.6f,0.6f,1));
	}

	mLayerButtons->get(layer)->getRenderer()->setColor(Vector4(-1.0f,0,0.5f,1));

	mMapEditor->mLayer = layer;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
