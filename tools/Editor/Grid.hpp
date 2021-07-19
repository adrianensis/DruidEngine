#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"

#include <vector>

class GameObject;
class EditorController;

CLASS(CellGrid, ObjectBase)
{
	PUB(GameObject, NONE, GameObject *)
public:

};

CLASS(Grid, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(CellsGrid, NONE, std::vector<std::vector<CellGrid>>)
	PRI(GridSize, GETREF_CONST, Vector2)
	PRI(TileSize, GETREF_CONST, Vector2)

public:

	void init(EditorController* editorController);
	void setCell(const Vector2& gridPosition, GameObject* tile);
	CellGrid& getCell(const Vector2& gridPosition);

	bool hasTile(const Vector2& gridPosition) const;
	bool isInGrid(const Vector2& position) const;
	
	Vector2 calculateGridPosition(const Vector2& position) const;
	Vector2 calculateClampedPosition(const Vector2& position) const;
};