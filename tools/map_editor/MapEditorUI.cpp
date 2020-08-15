#include "MapEditorUI.hpp"
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

const std::string MapEditorUI::StringsUI::smTile = "Tile";
const std::string MapEditorUI::StringsUI::smZoom = "Zoom";
const std::string MapEditorUI::StringsUI::smBrush = "Brush";
const std::string MapEditorUI::StringsUI::smSave = "Save";
const std::string MapEditorUI::StringsUI::smCollider = "Colliders";
const std::string MapEditorUI::StringsUI::smAtlas = "Atlas";
const std::string MapEditorUI::StringsUI::smPlay = "Play";
const std::string MapEditorUI::StringsUI::smCreateSprite = "Create Sprite";
const std::string MapEditorUI::StringsUI::smInspectorTileX = "x:";
const std::string MapEditorUI::StringsUI::smInspectorTileY = "y:";
const std::string MapEditorUI::StringsUI::smInspectorTileCollider = "collider:";
const std::string MapEditorUI::StringsUI::smInspectorTileTag = "tag:";
const std::string MapEditorUI::StringsUI::smInspectorSize = "size:";
const std::string MapEditorUI::StringsUI::smInspectorSizeCollider = "collider size:";
const std::string MapEditorUI::StringsUI::smGrid = "Grid";

// ---------------------------------------------------------------------------

MapEditorUI::MapEditorUI(){

}

MapEditorUI::~MapEditorUI(){
	/*Memory::free<Array<UIButton*>>(mAtlasButtons);
	Memory::free<Array<UIButton*>>(mSpriteButtons);*/
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
}

// ---------------------------------------------------------------------------

void MapEditorUI::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 1;
	f32 baseX = -1.4f;
	f32 baseY = 0.92f;
	f32 sizeChar = 0.04f;

	UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(Vector2(sizeChar * StringsUI::smSave.length(), 0.1f))->
		setText(StringsUI::smSave)->
		setLayer(mUILayer)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, self = button]() {
		self->getScene()->saveScene(self->getScene()->getPath());
	});

	button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * StringsUI::smCollider.length(), 0.1f))->
		setText(StringsUI::smCollider)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, self = button]() {
		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());

	});

	button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * StringsUI::smAtlas.length(), 0.1f))->
		setText(StringsUI::smAtlas)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, self = button, mapEditorUI = this]() {
		mapEditorUI->toggleAtlas();
	});

	button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * StringsUI::smPlay.length(), 0.1f))->
		setText(StringsUI::smPlay)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, self = button]() {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			 mMapEditor->createPlayer();
		}

		 mMapEditor->switchCameraControl();
	});

	button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * StringsUI::smGrid.length(), 0.1f))->
		setText(StringsUI::smGrid)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, mapEditorUI = this]() {
		mapEditorUI->toggleGrid();

	});

	button = (UIButton*) UI::getInstance()->getBuilder()->
		setSize(Vector2(sizeChar * StringsUI::smCreateSprite.length(), 0.1f))->
		setText(StringsUI::smCreateSprite)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback([&, mapEditorUI = this]() {
		//mapEditorUI->toggleGrid();

	});
}

// ---------------------------------------------------------------------------

void MapEditorUI::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 0;
	f32 baseX = 0.84f;
	f32 baseY = 0.92f;
	f32 separatorSize = 0.052f;

	mTextInspectorTag = (UIText*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(mTextSize)->
		setText(StringsUI::smInspectorTileTag)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxTag = (UIText*) UI::getInstance()->getBuilder()->
		setText("")->
		create(UIElementType::TEXTBOX)->
		getUIElement();

	mTextBoxTag->setOnTextChangedCallback([self = mTextBoxTag, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string tag(self->getInputString());
			mapEditor->mSelectedTile->setTag(tag);
		}
	});

	mTextInspectorX = (UIText*) UI::getInstance()->getBuilder()->
		setText(StringsUI::smInspectorTileX + "0.000000")->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextInspectorY = (UIText*) UI::getInstance()->getBuilder()->
		setText(StringsUI::smInspectorTileY + "0.000000")->
		create(UIElementType::TEXT)->
		getUIElement();


	mTextInspectorY = (UIText*) UI::getInstance()->getBuilder()->
		setText(StringsUI::smInspectorTileCollider)->
		create(UIElementType::TEXT)->
		getUIElement();



	f32 buttonSize = 0.15f;

	mButtonInspectorCollider = (UIButton*) UI::getInstance()->getBuilder()->
			setSize(Vector2(buttonSize, 0.05f))->
			setText("[ ]")->
			create(UIElementType::BUTTON)->
			getUIElement();

	mButtonInspectorCollider->setOnPressedCallback([&, self = mButtonInspectorCollider, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			List<Collider*>* colliders = mapEditor->mSelectedTile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				mapEditor->mSelectedTile->removeComponent<Collider>(colliders->get(0));
				mapEditor->mSelectedTile->removeComponent<RigidBody>(mapEditor->mSelectedTile->getComponents<RigidBody>()->get(0));
				self->setText("[ ]");
			} else {
				mapEditor->addColliderToTile(mapEditor->mSelectedTile);
				self->setText("[X]");
			}
		}
	});

	mTextTileSize = (UIText*) UI::getInstance()->getBuilder()->
		setSize(mTextSize)->
		setText(StringsUI::smInspectorSize)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxSizeX = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTBOX)->
		getUIElement();

	mTextBoxSizeX->setOnTextChangedCallback([self = mTextBoxSizeX, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string width(self->getInputString());

			Transform* tileTransform = mapEditor->mSelectedTile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(std::stof(width.length() > 0 ? width : "0"), scale.y, scale.z));
		}
	});

	mTextBoxSizeY = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTBOX)->
		getUIElement();

	mTextBoxSizeY->setOnTextChangedCallback([self = mTextBoxSizeY, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string height(self->getInputString());

			Transform* tileTransform = mapEditor->mSelectedTile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(scale.x, std::stof(height.length() > 0 ? height : "0"), scale.z));
		}
	});

	mTextColliderSize = (UIText*) UI::getInstance()->getBuilder()->
		setText(StringsUI::smInspectorSizeCollider)->
		setLayer(mUILayer)->
		create(UIElementType::TEXT)->
		getUIElement();

	mTextBoxColliderSizeX = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTBOX)->
		getUIElement();

	mTextBoxColliderSizeX->setOnTextChangedCallback([self = mTextBoxColliderSizeX, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string width(self->getInputString());

			auto colliderList = mapEditor->mSelectedTile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider)
				collider->setSize(std::stof(width.length() > 0 ? width : "0"), collider->getHeight());
		}
	});

	mTextBoxColliderSizeY = (UIText*) UI::getInstance()->getBuilder()->
		setText("0.0")->
		setLayer(mUILayer)->
		create(UIElementType::TEXTBOX)->
		getUIElement();

	mTextBoxColliderSizeY->setOnTextChangedCallback([self = mTextBoxColliderSizeY, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string height(self->getInputString());

			auto colliderList = mapEditor->mSelectedTile->getComponents<Collider>();
			Collider* collider = colliderList ? colliderList->get(0) : nullptr;

			if(collider)
				collider->setSize(collider->getWidth(), std::stof(height.length() > 0 ? height : "0"));
		}
	});
}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspector() {

	if(mMapEditor->mSelectedTile){
		Transform* tileTransform = mMapEditor->mSelectedTile->getTransform();

		mTextInspectorX->setText(StringsUI::smInspectorTileX + std::to_string(tileTransform->getLocalPosition().x));
		mTextInspectorY->setText(StringsUI::smInspectorTileY + std::to_string(tileTransform->getLocalPosition().y));

		List<Collider*>* colliders = mMapEditor->mSelectedTile->getComponents<Collider>();
		bool hasCollider = colliders && !colliders->isEmpty();

		mButtonInspectorCollider->setText(hasCollider ? "[X]" : "[ ]");
	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspectorOnSelectTile() {

	if(mMapEditor->mSelectedTile){
		updateInspector();

		std::string tag = mMapEditor->mSelectedTile->getTag();
		mTextBoxTag->setText(/*tag.length() > 0 ?*/ tag /*: "none"*/);

		Transform* tileTransform = mMapEditor->mSelectedTile->getTransform();

		mTextBoxSizeX->setText(std::to_string(tileTransform->getScale().x));
		mTextBoxSizeY->setText(std::to_string(tileTransform->getScale().y));

		auto colliderList = mMapEditor->mSelectedTile->getComponents<Collider>();
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

	/*mAtlasButtons = Memory::allocate<Array<UIButton*>>();
	mAtlasButtons->init(atlasSize.x*atlasSize.y);*/

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
				mapEditor->mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x,
						buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
						buttonRenderer->getRegionSize().y);

				Vector2 atlasPosition = Vector2(i, j);

				if((mapEditor->mBrush.mLastIndex < mapEditor->mBrush.mMaxGridSize) &&
						Input::isModifierPressed(GLFW_MOD_CONTROL)){
					mapEditor->mBrush.addTile(self, atlasPosition);
				} else {
					mapEditor->mBrush.clear();
					mapEditor->mBrush.addTile(self, atlasPosition);
				}

				mapEditor->mBrush.setDrawTileSize(mapEditor->mGridTileSize);
				mapEditor->mIsPaintMode = true;
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
	mLayerButtons = Memory::allocate<Array<UIButton*>>();
	mLayerButtons->init(maxLayers);

	Scene* scene = mMapEditor->getGameObject()->getScene();
	UIButton* button = nullptr;

	f32 size = 0.15f;
	f32 baseX = -1.4f;
	f32 baseY = 0.65f;

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::VERTICAL)->
		setPosition(Vector2(baseX, baseY))->
		setSize(Vector2(size,size))->
		setText("")->
		setLayer(mUILayer);

	FOR_RANGE(i, 0, mMapEditor->mConfigMap->getU32("atlases.length")){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		create(UIElementType::BUTTON)->
		getUIElement();


		Material* material = MaterialManager::getInstance()->loadMaterial(mMapEditor->mConfigMap->getString("atlases["+ std::to_string(i) +"].texture"));
		button->getRenderer()->setMaterial(material);

		button->setOnPressedCallback([&, i=i, material = material, mapEditorUI = this]() {
			mMapEditor->mMaterial = material;
			mMapEditor->createBrush(); // TODO : Move brush to UIMapEditor class
			UI::getInstance()->removeElementsFromGroup(mAtlasUIGroup);
			createAtlas(i, material);
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

			f32 pos = (i * mMapEditor->mGridTileSize) + mMapEditor->mGridTileSize/2.0f;

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
	f32 tileSize = 0.2f;
	Vector2 screenOffset = Vector2(-1.0f, 0.0f);
	f32 separator = 0.01f;
	Vector2 origin = Vector2(-1.2f, -0.7);

	Vector2 atlasSize = Vector2(4, 8);
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	f32 j = 0.5f;

	u32 spritesCount = 4;

	/*mSpriteButtons = Memory::allocate<Array<UIButton*>>();
	mSpriteButtons->init(spritesCount);*/

	UIElementData data;

	UIButton* tile = (UIButton*) UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(origin)->
		setSize(Vector2(tileSize, tileSize))->
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
			mapEditor->mBrush.mBrushCursor->getComponents<Renderer>()->get(0)->setRegion(buttonRenderer->getRegionPosition().x,
					buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
					buttonRenderer->getRegionSize().y);

			Vector2 atlasPosition = Vector2(0, 0);

			mapEditor->mBrush.clear();
			mapEditor->mBrush.addTile(self, atlasPosition);

			mapEditor->mBrush.setDrawTileSize(200);

			mapEditor->mIsPaintMode = true;
		});

		UI::getInstance()->addToGroup(mSpritesUIGroup, sprite);

		//mSpriteButtons->set(i, sprite);
	}
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
		mLayerButtons->get(i)->getRenderer()->setColor(Vector4(0,0,0,1));
	}

	mLayerButtons->get(layer)->getRenderer()->setColor(Vector4(-1.0f,0,0.5f,1));

	mMapEditor->mLayer = layer;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
