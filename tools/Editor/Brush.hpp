#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"

class Grid;
class GameObject;

CLASS(Brush, ObjectBase)
{
	PRI(Grid, NONE, Grid *)
	PRI(Selector, NONE, GameObject *)
	PRI(BrushPreview, NONE, GameObject *)

private:

	void createSelector();
	void setSelectorVisibility(bool visible);
	void createBrushPreview();
	void removeBrushPreview();
	GameObject* createSprite(const Vector2 &position, const Vector2 &size);

public:

	void init();
	void onPressed(const Vector2& position);
	void setGrid(Grid* grid);
};