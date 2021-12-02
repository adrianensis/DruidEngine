#pragma once

#include "Core/Core.hpp"
#include "EditorUIElement.hpp"

class GameObject;

class CellGrid: public ObjectBase
{
    GENERATE_METADATA(CellGrid)
	PUB GameObject* mGameObject = nullptr;
	PUB Vector2 mWorldPosition;
PUB

};

class Grid: public EditorUIElement
{
    GENERATE_METADATA(Grid)
	PRI std::vector<std::vector<CellGrid>> mCellsGrid;
	PRI Vector2 mGridSize; GETREF_CONST(GridSize)
	PRI Vector2 mTileSize; GETREF_CONST(TileSize)

PUB

	void init(EditorController* editorController) override;
	
	void setCell(const Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(const Vector2& gridPosition);

	bool hasTile(const Vector2& gridPosition) const;
	bool isInGrid(const Vector2& worldPosition) const;
	
	Vector2 calculateGridPosition(const Vector2& worldPosition) const;
	Vector2 calculateClampedPosition(const Vector2& worldPosition) const;
};