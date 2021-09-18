#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"


class GameObject;
class EditorController;

CLASS(CellGrid, ObjectBase)
{
	PUB(GameObject, NONE, GameObject *)
	PUB(WorldPosition, NONE, Vector2)
public:

};

CLASS(Grid, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(CellsGrid, NONE, SVec<SVec<CellGrid>>)
	PRI(GridSize, GETREF_CONST, Vector2)
	PRI(TileSize, GETREF_CONST, Vector2)

public:

	void init(EditorController* editorController);
	void setCell(CNS Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(CNS Vector2& gridPosition);

	bool hasTile(CNS Vector2& gridPosition) CNS;
	bool isInGrid(CNS Vector2& worldPosition) CNS;
	
	Vector2 calculateGridPosition(CNS Vector2& worldPosition) CNS;
	Vector2 calculateClampedPosition(CNS Vector2& worldPosition) CNS;
};