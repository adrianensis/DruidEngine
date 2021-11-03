#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "EditorUIElement.hpp"

class GameObject;

class CellGrid: public ObjectBase
{
    GENERATE_METADATA(CellGrid)
	PUB_M(GameObject *, GameObject, NONE)
	PUB_M(Vector2, WorldPosition, NONE)
PUB

};

class Grid: public EditorUIElement
{
    GENERATE_METADATA(Grid)
	PRI_M(SVec(SVec(CellGrid)), CellsGrid, NONE)
	PRI_M(Vector2, GridSize, GETREF_CONST)
	PRI_M(Vector2, TileSize, GETREF_CONST)

PUB

	void init(EditorController* editorController) override;
	
	void setCell(const Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(const Vector2& gridPosition);

	bool hasTile(const Vector2& gridPosition) const;
	bool isInGrid(const Vector2& worldPosition) const;
	
	Vector2 calculateGridPosition(const Vector2& worldPosition) const;
	Vector2 calculateClampedPosition(const Vector2& worldPosition) const;
};