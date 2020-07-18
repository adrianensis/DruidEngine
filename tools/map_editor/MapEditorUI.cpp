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

const std::string MapEditorUI::StringsUI::smLayer = "Layer";
const std::string MapEditorUI::StringsUI::smTile = "Tile";
const std::string MapEditorUI::StringsUI::smZoom = "Zoom";
const std::string MapEditorUI::StringsUI::smBrush = "Brush";
const std::string MapEditorUI::StringsUI::smSave = "Save";
const std::string MapEditorUI::StringsUI::smCollider = "Collider";
const std::string MapEditorUI::StringsUI::smAtlas = "Atlas";
const std::string MapEditorUI::StringsUI::smPlay = "Play";
const std::string MapEditorUI::StringsUI::smInspectorTileX = "x:";
const std::string MapEditorUI::StringsUI::smInspectorTileY = "y:";
const std::string MapEditorUI::StringsUI::smInspectorTileCollider = "collider:";
const std::string MapEditorUI::StringsUI::smInspectorTileTag = "tag:";
const std::string MapEditorUI::StringsUI::smSize = "size:";

// ---------------------------------------------------------------------------

MapEditorUI::MapEditorUI(){

}

MapEditorUI::~MapEditorUI(){
	Memory::free<Array<UIButton*>>(mAtlasButtons);
	Memory::free<Array<UIButton*>>(mSpriteButtons);
}

// ---------------------------------------------------------------------------

void MapEditorUI::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	Scene* scene = mMapEditor->getGameObject()->getScene();
	//mTextLayer = UI::getInstance()->getBuilder()->create(scene, Vector2(-1.3f, -0.85f), mTextSize, StringsUI::smLayer + " 0", mUILayer);

	UIElementData data;
	data.init(Vector2(-1.3f, -0.85f), mTextSize, StringsUI::smLayer + " 0", mUILayer);
	mTextLayer = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);

	createMenuBar();
	createInspector();
	createAtlas();
	createSprites();

	toggleAtlas();
}

// ---------------------------------------------------------------------------

void MapEditorUI::createMenuBar() {

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

	UIElementData data;
	data.init(Vector2(baseX, baseY), Vector2(sizeButton, 0.1f), str, mUILayer);

	UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::BUTTON);

	++i;
	button->setOnPressedCallback([&, self = button]() {
		self->getScene()->saveScene(self->getScene()->getPath());
	});
	sizeButtonAccumulated = sizeButton/2.0f;

	str = StringsUI::smCollider;
	sizeButton = sizeChar * str.length();

	data.init(Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), str, mUILayer);
	button = (UIButton*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::BUTTON);

	++i;
	button->setOnPressedCallback([&, self = button]() {
		RenderEngine::getInstance()->setDebugColliders(!RenderEngine::getInstance()->getDebugColliders());

	});
	sizeButtonAccumulated += sizeButton + separatorSize;

	str = StringsUI::smAtlas;
	sizeButton = sizeChar * str.length();

	data.init(Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), str, mUILayer);
	button = (UIButton*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::BUTTON);

	++i;
	button->setOnPressedCallback([&, self = button, mapEditorUI = this]() {
		mapEditorUI->toggleAtlas();
	});
	sizeButtonAccumulated += sizeButton + separatorSize;


	str = StringsUI::smPlay;
	sizeButton = sizeChar * str.length();
	data.init(Vector2(baseX + sizeButtonAccumulated + sizeButton/2.0f + separatorSize, baseY), Vector2(sizeButton, 0.1f), str, mUILayer);
	button = (UIButton*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::BUTTON);

	++i;
	button->setOnPressedCallback([&, self = button]() {
		if (! mMapEditor->mPlayer) {
			 mMapEditor->createPlayer();
		}

		 mMapEditor->switchCameraControl();
	});
	sizeButtonAccumulated += sizeButton + separatorSize;
}

// ---------------------------------------------------------------------------

void MapEditorUI::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	u32 i = 0;
	f32 baseX = 0.85f;
	f32 baseY = 0.9f;
	f32 separatorSize = 0.05f;
	f32 size = 0.2f;

	UIElementData data;
	data.init(Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileTag, mUILayer);
	mTextInspectorTag = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);
	++i;
	data.init(Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileX + "0.000000", mUILayer);
	mTextInspectorX = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);
	++i;
	data.init(Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileY + "0.000000", mUILayer);
	mTextInspectorY = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);
	++i;
	data.init(Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smInspectorTileCollider, mUILayer);
	mTextInspectorCollider = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);

	f32 buttonSize = 0.15f;
	data.init(Vector2(1.3f, baseY - separatorSize * i), Vector2(buttonSize, 0.05f), "[ ]", mUILayer);
	mButtonInspectorCollider = (UIButton*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::BUTTON);
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

	++i;

	data.init(Vector2(baseX, baseY - separatorSize * i), mTextSize, StringsUI::smSize, mUILayer);
	mTextTileSize = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXT);

	++i;

	data.init(Vector2(baseX + separatorSize, baseY - separatorSize * i), mTextSize, "0.0", mUILayer);
	mTextBoxSizeX = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXTBOX);

	mTextBoxSizeX->setOnTextChangedFunctor([self = mTextBoxSizeX, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string coordXStr(self->getInputString());

			Transform* tileTransform = mapEditor->mSelectedTile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(std::stof(coordXStr), scale.y, scale.z));
		}
	});

	++i;

	data.init(Vector2(baseX + separatorSize, baseY - separatorSize * i), mTextSize, "0.0", mUILayer);
	mTextBoxSizeY = (UIText*) UI::getInstance()->getBuilder()->create(scene, data, UIElementType::TEXTBOX);

	mTextBoxSizeY->setOnTextChangedFunctor([self = mTextBoxSizeY, mapEditor = mMapEditor]() {
		if(mapEditor->mSelectedTile){
			std::string coordYStr(self->getInputString());

			Transform* tileTransform = mapEditor->mSelectedTile->getTransform();
			Vector3 scale = tileTransform->getScale();
			tileTransform->setScale(Vector3(scale.x, std::stof(coordYStr), scale.z));
		}
	});

	++i;
}

// ---------------------------------------------------------------------------

void MapEditorUI::updateInspector() {

	if(mMapEditor->mSelectedTile){
		Transform* tileTransform = mMapEditor->mSelectedTile->getTransform();

		mTextInspectorTag->setText(StringsUI::smInspectorTileTag + mMapEditor->mSelectedTile->getTag());
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

		Transform* tileTransform = mMapEditor->mSelectedTile->getTransform();

		mTextBoxSizeX->setText(std::to_string(tileTransform->getScale().x));
		mTextBoxSizeY->setText(std::to_string(tileTransform->getScale().y));
	}
}

// ---------------------------------------------------------------------------

void MapEditorUI::createAtlas() {

	f32 tileSize = 0.07f;

	Vector2 screenOffset = Vector2(-1.0f, 0.0f);

	Vector2 atlasSize = Vector2(8, 16);
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	mAtlasButtons = Memory::allocate<Array<UIButton*>>();
	mAtlasButtons->init(atlasSize.x*atlasSize.y);

	UIElementData data;

	FOR_RANGE(i, 0, atlasSize.x)
	{
		FOR_RANGE(j, 0, atlasSize.y)
		{
			data.init(Vector2((i - (atlasSize.x / 2.0f)) * tileSize + screenOffset.x,
					((atlasSize.y / 2.0f) - j) * tileSize - screenOffset.y), Vector2(tileSize, tileSize), "", mUILayer);

			UIButton* tile = (UIButton*) UI::getInstance()->getBuilder()->create(
					mMapEditor->getGameObject()->getScene(), data, UIElementType::BUTTON);

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(mMapEditor->mMaterial);
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

			mAtlasButtons->set(i*atlasSize.y + j, tile);
		}
	}

	// Background
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
	renderer->setMaterial(MaterialManager::getInstance()->loadMaterial("resources/button.png"));

	mMapEditor->getGameObject()->getScene()->addGameObject(mAtlasBackground);
}

// ---------------------------------------------------------------------------

void MapEditorUI::update() {
	mTextLayer->setText(StringsUI::smLayer + " " + std::to_string(mMapEditor->mLayer));

	updateInspector();
}

// ---------------------------------------------------------------------------

void MapEditorUI::toggleAtlas(){
	mIsAtlasShow = !mIsAtlasShow;

	FOR_ARRAY(i, mAtlasButtons){
		mAtlasButtons->get(i)->getRenderer()->setIsActive(mIsAtlasShow);
		mAtlasButtons->get(i)->getCollider()->setIsActive(mIsAtlasShow);
	}

	mAtlasBackground->getComponents<Renderer>()->get(0)->setIsActive(mIsAtlasShow);

	FOR_ARRAY(i, mSpriteButtons){
		mSpriteButtons->get(i)->getRenderer()->setIsActive(mIsAtlasShow);
		mSpriteButtons->get(i)->getCollider()->setIsActive(mIsAtlasShow);
	}
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

	mSpriteButtons = Memory::allocate<Array<UIButton*>>();
	mSpriteButtons->init(spritesCount);

	UIElementData data;

	FOR_RANGE(i, 0, spritesCount){

		data.init(Vector2(origin.x + i*(tileSize + separator), origin.y), Vector2(tileSize, tileSize), "", mUILayer);


		UIButton* sprite = (UIButton*) UI::getInstance()->getBuilder()->create(
				mMapEditor->getGameObject()->getScene(), data, UIElementType::BUTTON);

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

		mSpriteButtons->set(i, sprite);
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
