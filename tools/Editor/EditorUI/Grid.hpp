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
	void setCell(CNS Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(CNS Vector2& gridPosition);

	bool hasTile(CNS Vector2& gridPosition) CNS;
	bool isInGrid(CNS Vector2& worldPosition) CNS;
	
	Vector2 calculateGridPosition(CNS Vector2& worldPosition) CNS;
	Vector2 calculateClampedPosition(CNS Vector2& worldPosition) CNS;
};