#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"


class GameObject;
class EditorController;

CLASS(CellGrid, ObjectBase)
{
	PUB(GameObject *, GameObject, NONE)
	PUB(Vector2, WorldPosition, NONE)
public:

};

CLASS(Grid, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
	PRI(SVec(SVec(CellGrid)), CellsGrid, NONE)
	PRI(Vector2, GridSize, GETREF_CONST)
	PRI(Vector2, TileSize, GETREF_CONST)

public:

	void init(EditorController* editorController);
	void setCell(const Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(const Vector2& gridPosition);

	bool hasTile(const Vector2& gridPosition) const;
	bool isInGrid(const Vector2& worldPosition) const;
	
	Vector2 calculateGridPosition(const Vector2& worldPosition) const;
	Vector2 calculateClampedPosition(const Vector2& worldPosition) const;
};