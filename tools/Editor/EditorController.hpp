#pragma once

#include "Core/ObjectBase.hpp"
#include "Brush.hpp"
#include "Grid.hpp"
#include "Atlas.hpp"

#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"

class Material;

CLASS(EditorController, ObjectBase)
{
	PRI(Brush, GETREF, Brush)
	PRI(Grid, GETREF, Grid)
	PRI(Atlas, GETREF, Atlas)

public:

	void init();
	void drawGrid() const;
	GameObject* createTile(const Vector2 &position, const Vector2 &size, Material *material, const Rectangle &region);

	bool canUseBrush() const;
};