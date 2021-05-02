#include "MapEditorUI.hpp"

#include "Core/TimeUtils.hpp"

#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"

#include "Config/EngineConfig.hpp"

#include "Core/TimerManager.hpp"

#include "Scene/GameObjectBuilder.hpp"
#include "Grid/Grid.hpp"
#include "MapEditor.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

void MapEditorUI::resetBrush() {
	mMapEditor->mMapEditorUI.mBrush.setIsPaintMode(false);
	mBrush.getBrushCursor()->getFirstComponent<Renderer>()->setRegion(0, 0, 1, 1);
	mBrush.clear();
}

void MapEditorUI::createBrush() {
	mBrush.init(mMapEditor);
}


MapEditorUI::MapEditorUI(){

}

MapEditorUI::~MapEditorUI(){
	Memory::free(mInspector);
}

void MapEditorUI::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	UI::getInstance()->getBuilder()->setScene(mMapEditor->getGameObject()->getScene()); // TODO: Move this to a more "Core" place
	//UI::getInstance()->getBuilder()->setTextSize(UI::getInstance()->getDefaultFontSize() / 1.2f);

	createMenuBar();
	createInspector();
	createAtlas();
	createInfoBar();
	createBrush();

	mSpriteButtons = Memory::allocate<List<UIButton*>>();
	mSpriteButtons->init();
}

void MapEditorUI::createMenuBar() {

	mMenuBar = Memory::allocate<MenuBar>();
	mMenuBar->init(mMapEditor);
}

void MapEditorUI::createInspector() {
	mInspector = Memory::allocate<Inspector>();
	mInspector->init(mMapEditor);
}

void MapEditorUI::createInfoBar() {

	// TODO: move to an isolated component.

	f32 baseY = -0.9f;

	EditorBuilder::getInstance()->createPanel(Vector2(-1* RenderContext::getAspectRatio(),baseY), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-0.9f * RenderContext::getAspectRatio(), baseY))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mUILayer);

	mTextFPS = EditorBuilder::getInstance()->createLabel(StringsUI::getInstance()->FPS);
}

void MapEditorUI::updateInfoBar() {
	mTextFPS->setText(StringsUI::getInstance()->FPS + std::to_string(1.0f/Time::getInstance()->getDeltaTimeSeconds()));
}

void MapEditorUI::createAtlas() {
	mAtlas = Memory::allocate<Atlas>();
	mAtlas->init(mMapEditor);
}

void MapEditorUI::update() {
	mBrush.update();
	mInspector->update();
	updateGridLines();
	updateInfoBar();
}

void MapEditorUI::updateGridLines() {
	if(mIsGridShow){
		f32 halfLinesCount = 30;
		f32 lineLength = 5000;

		RenderEngine::getInstance()->drawLine(Vector3(-lineLength,0,0), Vector3(lineLength,0,0), 3, true);
		RenderEngine::getInstance()->drawLine(Vector3(0,-lineLength,0), Vector3(0,lineLength,0), 3, true);

		// GRID LINES
		FOR_RANGE(i, -halfLinesCount, halfLinesCount){
			f32 pos = (i * mMapEditor->mGrid.getTileSize()) + mMapEditor->mGrid.getTileSize()/2.0f;
			RenderEngine::getInstance()->drawLine(Vector3(-lineLength,pos,0), Vector3(lineLength,pos,0), 1, true);
			RenderEngine::getInstance()->drawLine(Vector3(pos,-lineLength,0), Vector3(pos,lineLength,0), 1, true);
		}
	}
}

void MapEditorUI::toggleGrid() {
	mIsGridShow = !mIsGridShow;
}

void MapEditorUI::toggleAtlas(){
	mAtlas->toggleAtlas();
}

void MapEditorUI::createSpriteFromBrush() {

	f32 spritesCount = mSpriteButtons->getLength();
	f32 size = 0.1f;
	f32 offsetX = size * spritesCount;

	GameObject* tileMin = mBrush.getTile(0,0);
	GameObject* tileMax = mBrush.getTile(mBrush.getBrushGridSize().y-1, mBrush.getBrushGridSize().x-1);

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-1.0f * RenderContext::getAspectRatio() + offsetX, -0.8f))->
		setSize(Vector2(size, size))->
		setAdjustSizeToText(false)->
		setLayer(mUILayer);

	UIButton* spriteButton = (UIButton*) UI::getInstance()->getBuilder()->setText("")->
		create(UIElementType::BUTTON)->getUIElement();

		Vector2 tileSize(mMapEditor->mGrid.getTileSize() * mBrush.getBrushGridSize().x,
											mMapEditor->mGrid.getTileSize() * mBrush.getBrushGridSize().y);

	spriteButton->setOnPressedCallback([&, self = spriteButton, mapEditor = mMapEditor, tileSize = tileSize](UIElement* uiElement) {
		//scene->saveScene(scene->getPath());
		mBrush.clear();
		mBrush.clickTile(self, Vector2(0,0));

		mBrush.setDrawTileSize(tileSize);
	});

	Renderer* brushRenderer = tileMin->getComponents<Renderer>()->get(0);

	Renderer* spriteButtonRenderer = spriteButton->getRenderer();

	spriteButtonRenderer->setMaterial(brushRenderer->getMaterial());

	spriteButtonRenderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y,
		brushRenderer->getRegionSize().x * mBrush.getBrushGridSize().x, brushRenderer->getRegionSize().y * mBrush.getBrushGridSize().y);

	UI::getInstance()->addToGroup(mSpritesUIGroup, spriteButton);

	mSpriteButtons->pushBack(spriteButton);

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
			mBrush.mBrushCursor->getFirstComponent<Renderer>()->setRegion(buttonRenderer->getRegionPosition().x,
					buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
					buttonRenderer->getRegionSize().y);

			Vector2 atlasPosition = Vector2(0, 0);

			mBrush.clear();
			mBrush.addTile(self, atlasPosition);

			mBrush.setDrawTileSize(200); // TODO : get this value from a config.

			mapEditor->mMapEditorUI.mBrush.mIsPaintMode = true;
		});

		UI::getInstance()->addToGroup(mSpritesUIGroup, sprite);

		//mSpriteButtons->set(i, sprite);
	}*/
}

}
