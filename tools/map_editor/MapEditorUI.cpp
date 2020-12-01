#include "MapEditorUI.hpp"

#include "TimeUtils.hpp"

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
#include "UITextEditable.hpp"

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

void Brush::clickTile(UIButton* tileButton, Vector2 atlasPosition){
	if(mLastIndex < mBrushMaxGridSize){

		// If it's the first selected tile, take it as coordinates origin.
		if(! mGrid->get(0)){
			mOriginAtlasPosition = atlasPosition;
		}

		Vector2 distance(std::abs(atlasPosition.x - mOriginAtlasPosition.x),
				std::abs(atlasPosition.y - mOriginAtlasPosition.y));

		mBrushGridSize.x = std::max(mBrushGridSize.x,distance.x + 1);
		mBrushGridSize.y = std::max(mBrushGridSize.y,distance.y + 1);

		mGrid->set(mLastIndex,tileButton);
		mLastIndex++;
		tileButton->getRenderer()->setColor(Vector4(0.2f,0.2f,0.2f,1));
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

void Brush::setDrawTileSize(const Vector2& size) {
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
	f32 gridTileSize = mMapEditor->mGrid.getGridTileSize();
	mBrush.setDrawTileSize(Vector2(gridTileSize, gridTileSize));
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
	createLayersBar();

	toggleAtlas();

	createBrush();

	DE_SUBSCRIBE_TO_EVENT(MapEditorOnSelectTile, [this](const Event* event){
		updateInspectorOnSelectTile();
	});
}

// ---------------------------------------------------------------------------

void MapEditorUI::createPanel(const Vector2& position, const Vector2 &size) {
	UI::getInstance()->getBuilder()->
	setLayout(UILayout::VERTICAL)->
	setPosition(position)->
	setLayer(mUILayer-1)->
	setAdjustSizeToText(false)->
	setSize(size)->
	create(UIElementType::PANEL);
}

void MapEditorUI::addMenuEntry(const std::string &title, std::function<void()> onPressedCallback) {
	UI::getInstance()->getBuilder()->setText(title)->
	create(UIElementType::BUTTON)->getUIElement()->
	setOnPressedCallback(onPressedCallback);
}

void MapEditorUI::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	createPanel(Vector2(0,0.92f), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-0.9f * RenderContext::getAspectRatio(), 0.92f))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mUILayer);

	addMenuEntry(mStringsUI.Save,[&]() {
		scene->saveScene(scene->getPath());
	});

	addMenuEntry(mStringsUI.Collider,[&]() {
		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());
	});

	addMenuEntry(mStringsUI.Atlas,[&, mapEditorUI = this]() {
		mapEditorUI->toggleAtlas();
	});

	addMenuEntry(mStringsUI.Play,[&]() {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			mMapEditor->createPlayer();
		}

		mMapEditor->switchCameraControl();
	});

	addMenuEntry(mStringsUI.Grid,[&, mapEditorUI = this]() {
		mapEditorUI->toggleGrid();
	});

	addMenuEntry(mStringsUI.AddSprite,[&, mapEditorUI = this]() {
		mapEditorUI->createSpriteFromBrush();
	});

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

UIText* MapEditorUI::createInspectorLabel(const std::string& text) {
	return (UITextEditable*) UI::getInstance()->getBuilder()->
		setText(text)->
		create(UIElementType::TEXT)->
		getUIElement();
}

UITextEditable* MapEditorUI::createInspectorTextBoxSimple(const std::string& text, std::function<void(UIElement* uiElement)> onTextChangedCallback) {

	UITextEditable* textEditable = (UITextEditable*) UI::getInstance()->getBuilder()->
		setText(text)->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	textEditable->setOnTextChangedCallback(onTextChangedCallback);

	return textEditable;
}

UITextEditable* MapEditorUI::createInspectorTextBoxLabeled(const std::string& textLabel, const std::string& text, std::function<void(UIElement* uiElement)> onTextChangedCallback) {

	createInspectorLabel(textLabel);
	UITextEditable* textEditable = createInspectorTextBoxSimple(text, onTextChangedCallback);
	return textEditable;
}

TextEditableVector2 MapEditorUI::createInspectorTextBoxVector2(const std::string& textLabel,
		std::function<void(UIElement* uiElement)> onTextChangedCallbackX, std::function<void(UIElement* uiElement)> onTextChangedCallbackY) {

	createInspectorLabel(textLabel);

	TextEditableVector2 textEditableVector2;
	std::string initValue = "0.0";
	textEditableVector2.TextEditableX = createInspectorTextBoxSimple(initValue, onTextChangedCallbackX);
	textEditableVector2.TextEditableY = createInspectorTextBoxSimple(initValue, onTextChangedCallbackY);

	return textEditableVector2;
}

void MapEditorUI::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseX = 0.7f * RenderContext::getAspectRatio();
	f32 baseY = 0.8f;
	f32 separatorSize = 0.052f;

	Vector2 panelSize(0.5f, 1.6f);
	Vector2 panelOffset(-0.1f,0.04f);

	createPanel(Vector2(baseX + panelSize.x/2.0f + panelOffset.x, baseY - panelSize.y/2.0f + panelOffset.y), panelSize);

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setAdjustSizeToText(true)->
		setPosition(Vector2(baseX, baseY))->
		setLayer(mUILayer);

	mTextBoxTag = createInspectorTextBoxLabeled(mStringsUI.InspectorTileTag, "",
	[&](UIElement* uiElement) {
		mMapEditor->mGrid.forEachSelectedTile(
			[&](GameObject* tile){
			std::string tag(uiElement->getInputString());
			tile->setTag(tag);
		});
	});

	mTextInspectorX = createInspectorLabel(mStringsUI.InspectorTileX + "0.000000");
	mTextInspectorY = createInspectorLabel(mStringsUI.InspectorTileY + "0.000000");





	createInspectorLabel(mStringsUI.InspectorTileCollider);

	mButtonInspectorCollider = (UIButton*) UI::getInstance()->getBuilder()->
		setText("[ ]")->
		create(UIElementType::BUTTON)->
		getUIElement();

	mButtonInspectorCollider->setOnPressedCallback([&, self = mButtonInspectorCollider]() {
		mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
			List<Collider*>* colliders = tile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				tile->removeComponent<Collider>(colliders->get(0));
				tile->removeComponent<RigidBody>(tile->getComponents<RigidBody>()->get(0));
				self->setText("[ ]");
			} else {
				mMapEditor->addColliderToTile(tile);
				self->setText("[X]");
			}
		});
	});





	TextEditableVector2 textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorSize,
			[&](UIElement* uiElement) {
				mMapEditor->mGrid.forEachSelectedTile(
				[&](GameObject* tile){
					std::string width(uiElement->getInputString());

					Transform* tileTransform = tile->getTransform();
					Vector3 scale = tileTransform->getScale();
					tileTransform->setScale(Vector3(std::stof(width.length() > 0 ? width : "0"), scale.y, scale.z));
					tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
				});
			},
			[&](UIElement* uiElement) {
				mMapEditor->mGrid.forEachSelectedTile(
				[&](GameObject* tile){
					std::string height(uiElement->getInputString());

					Transform* tileTransform = tile->getTransform();
					Vector3 scale = tileTransform->getScale();
					tileTransform->setScale(Vector3(scale.x, std::stof(height.length() > 0 ? height : "0"), scale.z));
					tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
				});
			}
	);

	mTextBoxSizeX = textEditableVector2.TextEditableX;
	mTextBoxSizeY = textEditableVector2.TextEditableY;




	textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorPosCollider,

		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				std::string x(uiElement->getInputString());

				auto colliderList = tile->getComponents<Collider>();
				Collider* collider = colliderList ? colliderList->get(0) : nullptr;

				if(collider){
					Vector3 offset = collider->getPositionOffset();
					offset.x = std::stof(x.length() > 0 ? x : "0");
					collider->setPositionOffset(offset);
				}
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				std::string y(uiElement->getInputString());

				auto colliderList = tile->getComponents<Collider>();
				Collider* collider = colliderList ? colliderList->get(0) : nullptr;

				if(collider){
					Vector3 offset = collider->getPositionOffset();
					offset.y = std::stof(y.length() > 0 ? y : "0");
					collider->setPositionOffset(offset);
				}
			});
		}
	);

	mTextBoxColliderPosX = textEditableVector2.TextEditableX;
	mTextBoxColliderPosY = textEditableVector2.TextEditableY;


	textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorSizeCollider,

		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				std::string width(uiElement->getInputString());

				auto colliderList = tile->getComponents<Collider>();
				Collider* collider = colliderList ? colliderList->get(0) : nullptr;

				if(collider)
					collider->setSize(std::stof(width.length() > 0 ? width : "0"), collider->getHeight());
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				std::string height(uiElement->getInputString());

				auto colliderList = tile->getComponents<Collider>();
				Collider* collider = colliderList ? colliderList->get(0) : nullptr;

				if(collider)
					collider->setSize(collider->getWidth(), std::stof(height.length() > 0 ? height : "0"));
			});
		}
	);

	mTextBoxColliderSizeX = textEditableVector2.TextEditableX;
	mTextBoxColliderSizeY = textEditableVector2.TextEditableY;

}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspector() {

	if(mMapEditor->mGrid.getFirstSelectedTile()){
		Transform* tileTransform = mMapEditor->mGrid.getFirstSelectedTile()->getTransform();

		mTextInspectorX->setText(mStringsUI.InspectorTileX + std::to_string(tileTransform->getLocalPosition().x));
		mTextInspectorY->setText(mStringsUI.InspectorTileY + std::to_string(tileTransform->getLocalPosition().y));

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
					mBrush.clickTile(self, atlasPosition);
				} else {
					mBrush.clear();
					mBrush.clickTile(self, atlasPosition);
				}

				f32 gridTileSize = mMapEditor->mGrid.getGridTileSize();
				mBrush.setDrawTileSize(Vector2(gridTileSize, gridTileSize)); // 1 unit per tile
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
	f32 baseX = -0.9f * RenderContext::getAspectRatio();
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

void MapEditorUI::createSpriteFromBrush() {

	GameObject* tileMin = mBrush.getTile(0,0);
	GameObject* tileMax = mBrush.getTile(mBrush.mBrushGridSize.y-1, mBrush.mBrushGridSize.x-1);

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-0.9f * RenderContext::getAspectRatio(), -0.92f))->
		setSize(Vector2(0.1f, 0.1f))->
		setAdjustSizeToText(false)->
		setLayer(mUILayer);

	UIButton* spriteButton = (UIButton*) UI::getInstance()->getBuilder()->setText("")->
		create(UIElementType::BUTTON)->getUIElement();

		Vector2 tileSize(mMapEditor->mGrid.getGridTileSize() * mBrush.mBrushGridSize.x,
											mMapEditor->mGrid.getGridTileSize() * mBrush.mBrushGridSize.y);

	spriteButton->setOnPressedCallback([&, self = spriteButton, mapEditor = mMapEditor, tileSize = tileSize]() {
		//scene->saveScene(scene->getPath());
		mBrush.clear();
		mBrush.clickTile(self, Vector2(0,0));

		mBrush.setDrawTileSize(tileSize);

		mapEditor->mGrid.mIsPaintMode = true;
	});

	Renderer* brushRenderer = tileMin->getComponents<Renderer>()->get(0);

	Renderer* spriteButtonRenderer = spriteButton->getRenderer();

	spriteButtonRenderer->setMaterial(brushRenderer->getMaterial());

	spriteButtonRenderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y,
		brushRenderer->getRegionSize().x * mBrush.mBrushGridSize.x, brushRenderer->getRegionSize().y * mBrush.mBrushGridSize.y);

	UI::getInstance()->addToGroup(mSpritesUIGroup, spriteButton);

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
	f32 baseX = 0.7f * RenderContext::getAspectRatio() + size + size/2.0f;
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
