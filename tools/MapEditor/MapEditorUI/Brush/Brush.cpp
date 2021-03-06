#include "Brush.hpp"

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
#include "Graphics/Animation/Animation.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"

#include "Config/EngineConfig.hpp"

#include "Core/TimerManager.hpp"

#include "Scene/GameObjectBuilder.hpp"
#include "../Grid/Grid.hpp"
#include "MapEditor.hpp"
#include "MapEditorUI/MapEditorUI.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

Brush::Brush() : DE_Class() {
	mMapElementData = nullptr;
}

Brush::~Brush() {
	free();
}

void Brush::init(MapEditor* mapEditor) {
	mMapEditor = mapEditor;
	mIsPaintMode = false;
}

void Brush::setMaterial(Material* material){
	if(material) {

		mMapElementData->mMaterial = material;

		free();

		mGrid = DE_NEW<Array<GameObject*>>();
		mGrid->init(mBrushMaxGridSize);

		mBrushCursor = GameObjectBuilder::getInstance()->
		createSprite(mMapElementData->mMaterial, mMapEditor->mMapEditorUI.mUILayer, false)->getGameObject();

		mBrushCursor->getTransform()->setLocalPosition(Vector3(0,0,0));
		mBrushCursor->getTransform()->setScale(Vector3(0.05f / RenderContext::getAspectRatio(), 0.05f, 1));
		mBrushCursor->getTransform()->setAffectedByProjection(false);

		mMapEditor->getGameObject()->getScene()->addGameObject(mBrushCursor);

		setMaterialRegion(Vector2(0,0), Vector2(1,1));
	} 
}

void Brush::setMaterialRegion(const Vector2& regionPosition, const Vector2& regionSize) {
	mMapElementData->mMaterialRegionPosition = regionPosition;
	mMapElementData->mMaterialRegionSize = regionSize;
	mBrushCursor->getFirstComponent<Renderer>()->setRegion(
		regionPosition.x, regionPosition.y,
		regionSize.x, regionSize.y);
}

GameObject* Brush::getTile(u32 i, u32 j){
	return mGrid ? mGrid->get(i * mBrushGridSize.x + j) : nullptr;
}

void Brush::clickTile(GameObject *tile, const Vector2& atlasPosition) {
	if(! ((mLastIndex < mBrushMaxGridSize) && Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL))){
		clear();		
	}

	setMaterial(tile->getFirstComponent<Renderer>()->getMaterial());

	addTile(tile, atlasPosition);

	f32 gridTileSize = mMapEditor->mGrid.getTileSize();
	setDrawTileSize(Vector2(gridTileSize, gridTileSize));
	mIsPaintMode = true;
}

void Brush::addTile(GameObject *tile, const Vector2& atlasPosition) {
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
		
		// change tile color to mark it as selected
		tile->getFirstComponent<Renderer>()->setColor(Vector4(0.2f,0.2f,0.2f,1));

		// change cursor tile
		Renderer* renderer = tile->getFirstComponent<Renderer>();
		setMaterialRegion(renderer->getRegionPosition(), renderer->getRegionSize());
	}
} 

void Brush::free(){
	if(mBrushCursor) {
		mMapEditor->getGameObject()->getScene()->removeGameObject(mBrushCursor);
		mBrushCursor = nullptr;
	}

	if(mGrid){
		DE_FREE(mGrid);
		mGrid = nullptr;
	}
}

void Brush::update(){
	if(mBrushCursor){
		Vector2 mouse = Input::getInstance()->getMousePosition();
		mBrushCursor->getTransform()->setLocalPosition(mouse);
	}
}

void Brush::clear(){
	FOR_RANGE(i, 0, mLastIndex){
		GameObject* tile = mGrid->get(i);

		if(tile)
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
	}

	mLastIndex = 0;
	mBrushGridSize = Vector2(1,1);
	mOriginAtlasPosition = Vector2(0,0);
	if(mGrid) {
		mGrid->clear();
	} 
}

void Brush::setDrawTileSize(const Vector2& size) {
	mDrawTileSize = size;
}

}
