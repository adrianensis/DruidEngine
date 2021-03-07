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
#include "Graphics/Animation/Animation.hpp"
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

void Grid::CellData::set(MapElement *gameObject, u32 layer) {
	if (!layers) {
		layers = DE_NEW<Array<MapElement*>>();
		layers->init(10); // MAX LAYERS
	}

	if (layer >= 0 && layer < 10) {
		layers->set(layer, gameObject);
	}
}

void Grid::CellData::remove(u32 layer) {
	if (layers) {
		if (layer >= 0 && layer < 10) {
			layers->set(layer, nullptr);
		}
	}
}

MapElement* Grid::CellData::get(u32 layer) {
	MapElement* gameObject = nullptr;

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

	DE_FREE(mSelectedMapElements);
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

	mSelectedMapElements = DE_NEW<List<MapElement*>>();
	mSelectedMapElements->init();
}

void Grid::update() {

	if(Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
	Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
		u32 sceneSize = mMapEditor->getGameObject()->getScene()->getSize() / 2.0f;

		Vector2 mouse(Input::getInstance()->getMousePosition());
		Vector3 world = mMapEditor->mCamera->screenToWorld(mouse);

		if(sceneSize >= std::abs(world.x) && sceneSize >= std::abs(world.y)){
			if(RenderEngine::getInstance()->getLayersData()->get(mMapEditor->mLayer)->mVisible){
				
				Vector3 clampedPosition(std::roundf(world.x / mTileSize) * mTileSize, std::roundf(world.y / mTileSize) * mTileSize, 0);
				
				if(mMapEditor->mMapEditorUI.mBrush.isGridSingleTile()) {
					f32 size = mMapEditor->mMapEditorUI.mBrush.mBrushSize;
					GameObject* brushTile = mMapEditor->mMapEditorUI.mBrush.getTile(0,0);
					if(size == 1) {
						Vector3 tilePosition = clampedPosition;
						click(tilePosition, brushTile);
					} else {
						f32 halfSize = size / 2.0f;
						FOR_RANGE(i, -halfSize, halfSize){
							FOR_RANGE(j, -halfSize, halfSize){
								Vector3 tilePosition = clampedPosition + Vector3(mTileSize*j, -mTileSize*i, 0);
								click(tilePosition, brushTile);
							}
						}
					}
				} else {
					FOR_RANGE(i, 0, mMapEditor->mMapEditorUI.mBrush.mBrushGridSize.y){
						FOR_RANGE(j, 0, mMapEditor->mMapEditorUI.mBrush.mBrushGridSize.x){
							Vector3 tilePosition = clampedPosition + Vector3(mTileSize*j, -mTileSize*i, 0);
							GameObject* brushTile = mMapEditor->mMapEditorUI.mBrush.getTile(i, j);
							click(tilePosition, brushTile);
						}
					}
				}
			}
		}
	}
}

void Grid::click(const Vector3 &clampedPosition, GameObject* brushTile) {

	f32 halfSizeX = (mGridSize * mTileSize / 2.0f);
	f32 halfSizeY = (mGridSize * mTileSize / 2.0f);
	
	if (std::fabs(clampedPosition.x) < halfSizeX && std::fabs(clampedPosition.y) < halfSizeY) {

		Vector2 gridPosition(clampedPosition.x / mTileSize + mGridSize / 2.0f,
				clampedPosition.y / mTileSize + mGridSize / 2.0f);

		if (gridPosition.x >= 0 && gridPosition.x < mGridSize && gridPosition.y >= 0 && gridPosition.y < mGridSize) {

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

			bool isPaintMode = mMapEditor->mMapEditorUI.mBrush.getIsPaintMode();
			u32 layer = mMapEditor->mLayer;

			if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if(isPaintMode){
					draw(brushTile, cellData, clampedPosition);
				} else {
					select(cellData, layer, Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL));
				}
			} else if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
				if(isPaintMode){
					remove(cellData, layer);
				}
			}
		}
	}
}

void Grid::select(CellData *cellData, u32 layer, bool multi) {

	if(!multi){
		FOR_LIST(it, mSelectedMapElements){
			if(it.get()){
				it.get()->getFirstComponent<Renderer>()->setColor(Vector4(0,0,0,1));
			}
		}

		mSelectedMapElements->clear();
	}

	if (cellData->get(layer)) {

		MapElement* tile = cellData->get(layer);
		if(mSelectedMapElements->find(tile).isNull()) {
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));

			mSelectedMapElements->pushBack(tile);

			EventOnSelectTile event;
			DE_SEND_EVENT(this, this, event);
		}
	}
}

MapElement* Grid::getFirstSelectedTile() {
	return mSelectedMapElements->isEmpty() ? nullptr : mSelectedMapElements->get(0);
}

void Grid::draw(GameObject* brushTile, CellData *cellData, const Vector3 &worldPosition) {

	u32 layer = mMapEditor->mLayer;

	MapElement* lastTile = cellData->get(layer);

	remove(cellData, layer);

	// This is generic data and need to be set here, just before creating the MapElement
	MapElementData* data = mMapEditor->mMapEditorUI.mBrush.mMapElementData;
	data->mPosition = worldPosition;
	data->mSize = mMapEditor->mMapEditorUI.mBrush.mDrawTileSize;
	data->mLayer = layer;
	data->mMaterialRegionPosition = brushTile->getFirstComponent<Renderer>()->getRegionPosition();
	data->mMaterialRegionSize = brushTile->getFirstComponent<Renderer>()->getRegionSize();

	MapElement* newMapElement = MapElement::create(data);
	mScene->addGameObject(newMapElement);

	cellData->set(newMapElement, layer);

	Renderer* renderer = newMapElement->getFirstComponent<Renderer>();

	renderer->setRegion(data->mMaterialRegionPosition.x, data->mMaterialRegionPosition.y,
		data->mMaterialRegionSize.x, data->mMaterialRegionSize.y);
}

void Grid::remove(CellData *cellData, u32 layer) {
	if (cellData->get(layer)) {
		mScene->removeGameObject(cellData->get(layer));
		cellData->remove(layer);
	}
}

void Grid::loadMapIntoGrid(const List<GameObject*>* gameObjects) {

	FOR_LIST (it, gameObjects){

		GameObject* gameObject = it.get();

		if (gameObject->isDerivedClass<MapElement>()) {

			MapElement* mapElement = gameObject->castTo<MapElement>();

			Transform* t = mapElement->getTransform();
			Vector3 worldPosition(t->getWorldPosition());
			Vector2 gridPosition(worldPosition.x / mTileSize + mGridSize/ 2.0f,
					worldPosition.y / mTileSize + mGridSize / 2.0f);

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
			cellData->set(mapElement, mapElement->getFirstComponent<Renderer>()->getLayer());
		}
	}
}

void Grid::forEachSelectedTile(std::function<void(MapElement*)> callback) {
	FOR_LIST(it, getSelectedMapElements()){
		MapElement* tile = it.get();
		if(tile){
			callback(tile);
		}
	}
}

}
