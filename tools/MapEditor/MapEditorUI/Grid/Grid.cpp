#include "Grid.hpp"

#include "Core/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Graphics/Shader.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "File/File.hpp"
#include "UI/UI.hpp"
#include "Config/EngineConfig.hpp"

#include "MapEditor.hpp"
#include "MapEditorUI/MapEditorUI.hpp"

namespace DE {

Grid::CellData::CellData() : DE_Class() {

}

Grid::CellData::~CellData() {
	if (layers) {
		DE_FREE(layers);
	}
}

void Grid::CellData::addGameObject(GameObject *gameObject, u32 layer) {
	if (!layers) {
		layers = DE_NEW<Array<GameObject*>>();
		layers->init(10); // MAX LAYERS
	}

	if (layer >= 0 && layer < 10) {
		layers->set(layer, gameObject);
	}
}

void Grid::CellData::removeGameObject(u32 layer) {
	if (layers) {
		if (layer >= 0 && layer < 10) {
			layers->set(layer, nullptr);
		}
	}
}

GameObject* Grid::CellData::get(u32 layer) {
	GameObject* gameObject = nullptr;

	if (layers) {
		gameObject = layers->get(layer);
	}

	return gameObject;
}

Grid::Grid() : DE_Class() {
	mScene = nullptr;
	mTileSize = 1;
}

Grid::~Grid() {

	if(mGrid){
		FOR_RANGE(i, 0, mGridSize){
			FOR_RANGE(j, 0, mGridSize){
				DE_FREE(mGrid->get(i)->get(j));
			}

			DE_FREE(mGrid->get(i));
		}
	}

	DE_FREE(mGrid);

	DE_FREE(mSelectedTiles);
}

void Grid::init(MapEditor* mapEditor, u32 gridSize, f32 tileSize) {
	mMapEditor = mapEditor;
	mScene = mapEditor->getGameObject()->getScene();
	mGridSize = gridSize;
	mTileSize = tileSize;

	mGrid = DE_NEW<Array<Array<CellData*>*>>();
	mGrid->init(mGridSize);

	FOR_RANGE(i, 0, mGridSize){
		mGrid->set(i, DE_NEW<Array<CellData*>>());
		mGrid->get(i)->init(mGridSize);
		FOR_RANGE(j, 0, mGridSize){
			CellData* cellData = DE_NEW<CellData>();
			mGrid->get(i)->set(j, cellData);
		}
	}

	mSelectedTiles = DE_NEW<List<GameObject*>>();
	mSelectedTiles->init();

	DE_SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event* event){
		u32 sceneSize = mMapEditor->getGameObject()->getScene()->getSize() / 2.0f;

		Vector2 mouse(Input::getInstance()->getMousePosition());
		Vector3 world = mMapEditor->mCamera->screenToWorld(mouse);

		Vector3 clampedPosition(std::roundf(world.x / mTileSize) * mTileSize, std::roundf(world.y / mTileSize) * mTileSize, 0);

		if(RenderEngine::getInstance()->getLayersData()->get(mMapEditor->mLayer)->mVisible){
			FOR_RANGE(i, 0, mMapEditor->mMapEditorUI.mBrush.mBrushGridSize.y){
				FOR_RANGE(j, 0, mMapEditor->mMapEditorUI.mBrush.mBrushGridSize.x){
					
					if(sceneSize >= std::abs(world.x) && sceneSize >= std::abs(world.y)){
						GameObject* brushTile = mMapEditor->mMapEditorUI.mBrush.getTile(i, j);
						Vector3 tilePosition = clampedPosition + Vector3(mTileSize*j, -mTileSize*i, 0);
						click(tilePosition, mMapEditor->mMapEditorUI.mBrush.mIsPaintMode, brushTile, mMapEditor->mMapEditorUI.mBrush.mDrawTileSizeFactor, mMapEditor->mLayer);
					}
				}
			}
		}
	});
}

void Grid::click(const Vector3 &clampedPosition, bool isPaintMode, GameObject* brushTile, const Vector2& tileSize, u32 layer) {

	f32 halfSizeX = (mGridSize * mTileSize / 2.0f);
	f32 halfSizeY = (mGridSize * mTileSize / 2.0f);
	
	if (std::fabs(clampedPosition.x) < halfSizeX && std::fabs(clampedPosition.y) < halfSizeY) {

		Vector2 gridPosition(clampedPosition.x / mTileSize + mGridSize / 2.0f,
				clampedPosition.y / mTileSize + mGridSize / 2.0f);

		if (gridPosition.x >= 0 && gridPosition.x < mGridSize && gridPosition.y >= 0 && gridPosition.y < mGridSize) {

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

			if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if(isPaintMode){
					drawTile(cellData, clampedPosition, brushTile, tileSize, layer);
				} else {
					selectTile(cellData, layer, Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL));
				}
			}

			if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
				if(isPaintMode){
					removeTile(cellData, layer);
				}
			}
		}
	}
}

void Grid::selectTile(CellData *cellData, u32 layer, bool multi) {

	if(!multi){
		FOR_LIST(it, mSelectedTiles){
			if(it.get()){
				it.get()->getFirstComponent<Renderer>()->setColor(Vector4(0,0,0,1));
			}
		}

		mSelectedTiles->clear();
	}

	if (cellData->get(layer)) {

		GameObject* tile = cellData->get(layer);
		if(mSelectedTiles->find(tile).isNull()) {
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));

			mSelectedTiles->pushBack(tile);

			EventOnSelectTile event;
			DE_SEND_EVENT(this, this, event);
		}
	}
}

GameObject* Grid::getFirstSelectedTile() {
	return mSelectedTiles->isEmpty() ? nullptr : mSelectedTiles->get(0);
}

bool Grid::isSameTile(GameObject* tileA, GameObject* tileB) {
	if(tileA && tileB){
		Renderer* rendererA = tileA->getComponents<Renderer>()->get(0);
		Renderer* rendererB = tileB->getComponents<Renderer>()->get(0);

		return rendererA->getMaterial() == rendererB->getMaterial() &&
				rendererA->getRegionPosition() == rendererB->getRegionPosition() &&
				rendererA->getRegionSize() == rendererB->getRegionSize();
	}

	return false;
}

void Grid::drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brushTile, const Vector2& size, u32 layer) {
	if(brushTile){

		GameObject* lastTile = cellData->get(layer);

		if (! isSameTile(brushTile, lastTile)) {
			removeTile(cellData, layer);

			Renderer* brushRenderer = brushTile->getComponents<Renderer>()->get(0);

			MapElement* newTile = MapElement::create(mScene, worldPosition, size, brushRenderer->getMaterial(), layer);
			cellData->addGameObject(newTile, layer);

			Renderer* renderer = newTile->getComponents<Renderer>()->get(0);

			renderer->setRegion(brushRenderer->getRegionPosition().x, brushRenderer->getRegionPosition().y,
				brushRenderer->getRegionSize().x, brushRenderer->getRegionSize().y);
		}
	}

}

void Grid::removeTile(CellData *cellData, u32 layer) {
	if (cellData->get(layer)) {
		mScene->removeGameObject(cellData->get(layer));
		cellData->removeGameObject(layer);
	}
}

void Grid::loadMapIntoGrid(const List<GameObject*>* gameObjects) {

	FOR_LIST (it, gameObjects){

		GameObject* gameObject = it.get();

		if (gameObject->getIsStatic() && gameObject->getTag() == "tile") {

			Transform* t = gameObject->getTransform();
			Vector3 worldPosition(t->getWorldPosition());
			Vector2 gridPosition(worldPosition.x / mTileSize + mGridSize/ 2.0f,
					worldPosition.y / mTileSize + mGridSize / 2.0f);

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
			cellData->addGameObject(gameObject, gameObject->getFirstComponent<Renderer>()->getLayer());
		}
	}
}

void Grid::forEachSelectedTile(std::function<void(GameObject*)> callback) {
	FOR_LIST(it, getSelectedTiles()){
		GameObject* tile = it.get();
		if(tile){
			callback(tile);
		}
	}
}

}
