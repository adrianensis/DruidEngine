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

#include "PhysicsEngine.hpp"
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
	//f32 gridTileSize = mMapEditor->mGrid.getGridTileSize();
	//mBrush.setDrawTileSize(Vector2(gridTileSize, gridTileSize));
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
	createInfoBar();
	toggleAtlas();
	createBrush();

	DE_SUBSCRIBE_TO_EVENT(MapEditorOnSelectTile, &(mapEditor->mGrid), this,
	[&](const Event* event){
		updateInspectorOnSelectTile();
	});
}

// ---------------------------------------------------------------------------

UIElement* MapEditorUI::createPanel(const Vector2& position, const Vector2 &size) {
	return UI::getInstance()->getBuilder()->
	setLayout(UILayout::VERTICAL)->
	setPosition(position)->
	setLayer(mUILayer-1)->
	setAdjustSizeToText(false)->
	setSize(size)->
	create(UIElementType::PANEL)->
	getUIElement();
}

void MapEditorUI::addMenuEntry(const std::string &title, UIElementCallback onPressedCallback) {
	UI::getInstance()->getBuilder()->setText(title)->
	create(UIElementType::BUTTON)->getUIElement()->
	setOnPressedCallback(onPressedCallback);
}

void MapEditorUI::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseY = 0.95f;

	createPanel(Vector2(-1 * RenderContext::getAspectRatio(),1), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-1 * RenderContext::getAspectRatio(), 1))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mUILayer);

	addMenuEntry(mStringsUI.Save,[&, scene = scene](UIElement* uiElement) {
		scene->saveScene(scene->getPath());
	});

	addMenuEntry(mStringsUI.Collider,[&](UIElement* uiElement) {
		PhysicsEngine::getInstance()->setDebugColliders(!PhysicsEngine::getInstance()->getDebugColliders());
	});

	addMenuEntry(mStringsUI.Atlas,[&, mapEditorUI = this](UIElement* uiElement) {
		mapEditorUI->toggleAtlas();
	});

	addMenuEntry(mStringsUI.Play,[&](UIElement* uiElement) {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			mMapEditor->createPlayer();
		}

		mMapEditor->switchCameraControl();
	});

	addMenuEntry(mStringsUI.Grid,[&, mapEditorUI = this](UIElement* uiElement) {
		mapEditorUI->toggleGrid();
	});

	addMenuEntry(mStringsUI.AddSprite,[&, mapEditorUI = this](UIElement* uiElement) {
		mapEditorUI->createSpriteFromBrush();
	});

	/*UIDropdown* uiDropdown = (UIDropdown*) UI::getInstance()->getBuilder()->
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

UIButton* MapEditorUI::createInspectorBoolean(const std::string& textLabel, UIElementCallback onPressedCallback) {
	createInspectorLabel(textLabel);

	UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->
		setText(mStringsUI.BoolFalse)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback(onPressedCallback);

	return button;
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

void MapEditorUI::setTileScaleFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex) {
	Transform* tileTransform = tile->getTransform();
	Vector3 scale = tileTransform->getScale();
	Vector3 newVector = Vector3(scale.x, scale.y, scale.z);
	newVector[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
	tileTransform->setScale(newVector);
	tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
}

void MapEditorUI::setTileColliderPositionFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex) {
	auto colliderList = tile->getComponents<Collider>();
	Collider* collider = colliderList ? colliderList->get(0) : nullptr;

	if(collider){
		Vector3 offset = collider->getPositionOffset();
		offset[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
		collider->setPositionOffset(offset);
	}
}

void MapEditorUI::setTileColliderScaleFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex) {
	auto colliderList = tile->getComponents<Collider>();
	Collider* collider = colliderList ? colliderList->get(0) : nullptr;

	if(collider) {
		Vector2 size = Vector2(collider->getWidth(), collider->getHeight());
		size[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
		collider->setSize(size.x, size.y);
	}
}

void MapEditorUI::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseX = 0.7f * RenderContext::getAspectRatio();
	f32 baseY = 0.8f;
	f32 separatorSize = 0.052f;

	Vector2 panelSize(0.5f, 1.8f);
	Vector2 panelPosition(1.0f * RenderContext::getAspectRatio() - panelSize.x, panelSize.y/2.0f);

	createPanel(panelPosition, panelSize);

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setAdjustSizeToText(true)->
		setPosition(Vector2(baseX, baseY))->
		setLayer(mUILayer);

	// Tag

	mTextBoxTag = createInspectorTextBoxLabeled(mStringsUI.InspectorTileTag, "",
	[&](UIElement* uiElement) {
		mMapEditor->mGrid.forEachSelectedTile(
		[&](GameObject* tile){
			tile->setTag(uiElement->getInputString());
		});
	});

	// Position

	mTextInspectorX = createInspectorLabel(mStringsUI.InspectorTileX + "0.000000");
	mTextInspectorY = createInspectorLabel(mStringsUI.InspectorTileY + "0.000000");

	// Collider enable

	mButtonInspectorCollider = createInspectorBoolean(mStringsUI.InspectorTileCollider,

	[&, self = mButtonInspectorCollider](UIElement* uiElement) {
		mMapEditor->mGrid.forEachSelectedTile(
		[&](GameObject* tile) {
			List<Collider*>* colliders = tile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				tile->removeComponent<Collider>(colliders->get(0));
				tile->removeComponent<RigidBody>(tile->getComponents<RigidBody>()->get(0));
				uiElement->setText(mStringsUI.BoolFalse);
			} else {
				mMapEditor->addColliderToTile(tile);
				uiElement->setText(mStringsUI.BoolTrue);
			}
		});
	});

	// Size

	TextEditableVector2 textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorSize,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile(
			[&](GameObject* tile) {
				setTileScaleFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile(
			[&](GameObject* tile) {
				setTileScaleFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxSizeX = textEditableVector2.TextEditableX;
	mTextBoxSizeY = textEditableVector2.TextEditableY;

	// Collider Position

	textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorPosCollider,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderPositionFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderPositionFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxColliderPosX = textEditableVector2.TextEditableX;
	mTextBoxColliderPosY = textEditableVector2.TextEditableY;

	// Collider Size

	textEditableVector2 = createInspectorTextBoxVector2(mStringsUI.InspectorSizeCollider,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderScaleFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderScaleFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxColliderSizeX = textEditableVector2.TextEditableX;
	mTextBoxColliderSizeY = textEditableVector2.TextEditableY;

}

// ---------------------------------------------------------------------------

void MapEditorUI::createInfoBar() {
	f32 baseY = -0.9f;

	createPanel(Vector2(-1* RenderContext::getAspectRatio(),baseY), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-0.9f * RenderContext::getAspectRatio(), baseY))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mUILayer);

	mTextFPS = createInspectorLabel(mStringsUI.FPS);
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

void MapEditorUI::updateInfoBar() {
	mTextFPS->setText(mStringsUI.FPS + std::to_string(1.0f/Time::getInstance()->getDeltaTimeSeconds()));
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

	Vector2 screenOffset = Vector2(tileSize/2.0f, tileSize/2.0f);

	Vector2 atlasSize = Vector2(mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.width"), mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.height"));
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	Vector2 panelSize = atlasSize * tileSize + tileSize;
	Vector2 panelPosition = Vector2(0.0f, 0.0f);
	UIElement* panel = createPanel(panelPosition, panelSize);
	UI::getInstance()->addToGroup(mAtlasUIGroup, panel);

	UI::getInstance()->getBuilder()->
		setSeparatorSize(0)->
		setText("");

	FOR_RANGE(i, 0, atlasSize.x){

		UI::getInstance()->getBuilder()->
			setLayout(UILayout::VERTICAL)->
			setAdjustSizeToText(false)->
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

			tile->setOnPressedCallback([&, self = tile, mapEditor = mMapEditor, i = i, j = j](UIElement* uiElement) {
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

	// TODO : create panel here
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
		setAdjustSizeToText(false)->
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

		button->setOnPressedCallback([&, i=i, material=material, this](UIElement* uiElement) {
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
	updateInfoBar();
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

	spriteButton->setOnPressedCallback([&, self = spriteButton, mapEditor = mMapEditor, tileSize = tileSize](UIElement* uiElement) {
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

	f32 separator = 0.08f;
	f32 baseX = 0.7f * RenderContext::getAspectRatio();
	f32 baseY = 0.0f;

	UI::getInstance()->getBuilder()->
		setAdjustSizeToText(true)->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX, baseY))->
		restoreColors()->
		setLayer(mUILayer);

	FOR_RANGE(i, 0, maxLayers){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText(std::to_string(i))->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, mapEditorUI = this](UIElement* uiElement) {
			mapEditorUI->selectLayer(i);
		});

		mLayerButtons->set(i, button);
	}

	UI::getInstance()->getBuilder()->
			setLayout(UILayout::VERTICAL)->
			setPosition(Vector2(baseX + separator, baseY));

	FOR_RANGE(i, 0, maxLayers){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText("on")->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, self = button](UIElement* uiElement) {
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
