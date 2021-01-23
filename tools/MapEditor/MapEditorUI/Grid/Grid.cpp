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
#include "Core/Settings.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Grid::CellData::CellData() : DE_Class() {

}

// ---------------------------------------------------------------------------

Grid::CellData::~CellData() {
	if (layers) {
		Memory::free<Array<GameObject*>>(layers);
	}
}

// ---------------------------------------------------------------------------

void Grid::CellData::addGameObject(GameObject *gameObject, u32 layer) {
	if (!layers) {
		layers = Memory::allocate<Array<GameObject*>>();
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

// ---------------------------------------------------------------------------

Grid::Grid() : DE_Class() {
	mScene = nullptr;
	mGridTileSize = 1;
}

// ---------------------------------------------------------------------------

Grid::~Grid() {

	if(mGrid){
		FOR_RANGE(i, 0, mGridSize){
			FOR_RANGE(j, 0, mGridSize){
				Memory::free<CellData>(mGrid->get(i)->get(j));
			}

			Memory::free<Array<CellData*>>(mGrid->get(i));
		}
	}

	Memory::free<Array<Array<CellData*>*>>(mGrid);

	Memory::free<List<GameObject*>>(mSelectedTiles);
}

// ---------------------------------------------------------------------------

void Grid::init(Scene* scene, u32 gridSize, f32 gridTileSize) {
	mScene = scene;
	mGridSize = gridSize;
	mGridTileSize = gridTileSize;

	mGrid = Memory::allocate<Array<Array<CellData*>*>>();
	mGrid->init(mGridSize);

	f32 halfGridSize = mGridSize / 2.0f;

	FOR_RANGE(i, 0, mGridSize){
		mGrid->set(i, Memory::allocate<Array<CellData*>>());
		mGrid->get(i)->init(mGridSize);
		FOR_RANGE(j, 0, mGridSize){
			CellData* cellData = Memory::allocate<CellData>();
			mGrid->get(i)->set(j, cellData);
		}
	}

	mSelectedTiles = Memory::allocate<List<GameObject*>>();
	mSelectedTiles->init();
}


// ---------------------------------------------------------------------------

void Grid::click(const Vector3 &clampedPosition, GameObject* brushTile, const Vector2& tileSize, u32 layer) {

	f32 halfSizeX = (mGridSize * mGridTileSize / 2.0f);
	f32 halfSizeY = (mGridSize * mGridTileSize / 2.0f);
	if (std::fabs(clampedPosition.x) < halfSizeX && std::fabs(clampedPosition.y) < halfSizeY) {

		Vector2 gridPosition(clampedPosition.x / mGridTileSize + mGridSize / 2.0f,
				clampedPosition.y / mGridTileSize + mGridSize / 2.0f);

		if (gridPosition.x >= 0 && gridPosition.x < mGridSize && gridPosition.y >= 0 && gridPosition.y < mGridSize) {

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);

			if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if(mIsPaintMode){
					drawTile(cellData, clampedPosition, brushTile, tileSize, layer);
				} else {
					selectTile(cellData, layer, Input::getInstance()->isModifierPressed(GLFW_MOD_CONTROL));
				}
			}

			if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

				if(mIsPaintMode){
					removeTile(cellData, layer);
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------

void Grid::selectTile(CellData *cellData, u32 layer, bool multi) {

	if(!multi){
		FOR_LIST(it, mSelectedTiles){
			if(it.get()/*mSelectedTile && mSelectedTile->isActive()*/){
				it.get()->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
			}
		}

		mSelectedTiles->clear();
	}

	if (cellData->get(layer)) {

		GameObject* tile = cellData->get(layer);
		if(mSelectedTiles->find(tile).isNull()) {
			tile->getComponents<Renderer>()->get(0)->setColor(Vector4(0.2f,0.2f,0.2f,1));

			mSelectedTiles->pushBack(tile);

			//mMapEditorUI.updateInspectorOnSelectTile();
			EventOnSelectTile event;
			DE_SEND_EVENT(this, this, event);
		}
	}
}

GameObject* Grid::getFirstSelectedTile() {
	return mSelectedTiles->isEmpty() ? nullptr : mSelectedTiles->get(0);
}

// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------

void Grid::removeTile(CellData *cellData, u32 layer) {
	if (cellData->get(layer)) {
		mScene->removeGameObject(cellData->get(layer));
		cellData->removeGameObject(layer);
	}
}

// ---------------------------------------------------------------------------

void Grid::loadMapIntoGrid(const List<GameObject*>* gameObjects) {

	FOR_LIST (it, gameObjects){

		GameObject* gameObject = it.get();

		if (gameObject->getIsStatic() && gameObject->getTag() == "tile") {

			Transform* t = gameObject->getTransform();
			Vector3 worldPosition(t->getWorldPosition());
			Vector2 gridPosition(worldPosition.x / mGridTileSize + mGridSize/ 2.0f,
					worldPosition.y / mGridTileSize + mGridSize / 2.0f);

			CellData* cellData = mGrid->get(gridPosition.x)->get(gridPosition.y);
			cellData->addGameObject(gameObject, gameObject->getComponents<Renderer>()->get(0)->getLayer());
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

} /* namespace DE */
