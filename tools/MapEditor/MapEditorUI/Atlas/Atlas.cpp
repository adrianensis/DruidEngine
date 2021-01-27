#include "Atlas.hpp"

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
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation.hpp"

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
#include "MapEditor.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

Atlas::Atlas(){

}

Atlas::~Atlas(){

}

void Atlas::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	createAtlasSelector();
	toggleAtlas();
}

void Atlas::createAtlas(u32 index, Material* material) {

	f32 tileSize = 0.07f;

	Vector2 screenOffset = Vector2(tileSize/2.0f, tileSize/2.0f);

	Vector2 atlasSize = Vector2(mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.width"), mMapEditor->mConfigMap->getU32("atlases["+ std::to_string(index) +"].size.height"));
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	Vector2 panelSize = atlasSize * tileSize;
	Vector2 panelPosition = Vector2(-panelSize.x/2.0f, panelSize.y/2.0f);
	UIElement* panel = EditorBuilder::getInstance()->createPanel(panelPosition, panelSize);
	UI::getInstance()->addToGroup(mAtlasUIGroup, panel);

	UI::getInstance()->getBuilder()->
		setSeparatorSize(0)->
		setAdjustSizeToText(false)->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-panelSize.x/2.0f, panelSize.y/2.0f))->
		setText("")->
		setSize(Vector2(tileSize, tileSize))->
		setBackgroundColor(Vector4(0,0,0,1))->
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	FOR_RANGE(i, 0, atlasSize.y){
		FOR_RANGE(j, 0, atlasSize.x){

			UIButton* tile = (UIButton*) UI::getInstance()->getBuilder()->
					create(UIElementType::BUTTON)->
					getUIElement();

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(material);
			renderer->setRegion(j / atlasSize.x, i / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y);

			tile->setOnPressedCallback([&, self = tile, mapEditor = mMapEditor, i = i, j = j](UIElement* uiElement) {
				Renderer* buttonRenderer = self->getRenderer();
				mMapEditor->mMapEditorUI.mBrush.mBrushCursor->getFirstComponent<Renderer>()->setRegion(buttonRenderer->getRegionPosition().x,
						buttonRenderer->getRegionPosition().y, buttonRenderer->getRegionSize().x,
						buttonRenderer->getRegionSize().y);

				Vector2 atlasPosition = Vector2(i, j);

				if((mMapEditor->mMapEditorUI.mBrush.mLastIndex < mMapEditor->mMapEditorUI.mBrush.mBrushMaxGridSize) &&
						Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL)){
					mMapEditor->mMapEditorUI.mBrush.clickTile(self, atlasPosition);
				} else {
					mMapEditor->mMapEditorUI.mBrush.clear();
					mMapEditor->mMapEditorUI.mBrush.clickTile(self, atlasPosition);
				}

				f32 gridTileSize = mMapEditor->mGrid.getGridTileSize();
				mMapEditor->mMapEditorUI.mBrush.setDrawTileSize(Vector2(gridTileSize, gridTileSize)); // 1 unit per tile
				mapEditor->mGrid.mIsPaintMode = true;
			});

			UI::getInstance()->addToGroup(mAtlasUIGroup, tile);

			//mAtlasButtons->set(i*atlasSize.y + j, tile);
		}

		UI::getInstance()->getBuilder()->nextRow();
	}

	UI::getInstance()->getBuilder()->restoreSeparatorSize();
}

void Atlas::createAtlasSelector() {
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
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	FOR_RANGE(i, 0, mMapEditor->mConfigMap->getU32("atlases.length")){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		create(UIElementType::BUTTON)->
		getUIElement();

		Material* material = MaterialManager::getInstance()->loadMaterial(mMapEditor->mConfigMap->getString("atlases["+ std::to_string(i) +"].texture"));
		button->getRenderer()->setMaterial(material);

		button->setOnPressedCallback([&, i=i, material=material, this](UIElement* uiElement) {
			mMapEditor->mMaterial = material;
			mMapEditor->mMapEditorUI.createBrush();
			UI::getInstance()->removeElementsFromGroup(mAtlasUIGroup);
			TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [this,i,material](){
				createAtlas(i, material);
			});
		});

		UI::getInstance()->addToGroup(mAtlasSelectorUIGroup, button);
	}
}

void Atlas::toggleAtlas(){
	mIsAtlasShow = !mIsAtlasShow;

	UI::getInstance()->setGroupVisibility(mAtlasUIGroup, mIsAtlasShow);
	UI::getInstance()->setGroupVisibility(mAtlasSelectorUIGroup, mIsAtlasShow);
	//UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsAtlasShow);
}

}
