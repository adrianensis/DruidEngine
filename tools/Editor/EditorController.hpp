#pragma once

#include "Core/ObjectBase.hpp"
#include "Brush.hpp"
#include "Grid.hpp"
#include "Atlas.hpp"
#include "EditorStyles.hpp"

#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"
#include <list>

class Material;
class GameObject;

using TileCallback = std::function<void(GameObject* tile)>;

CLASS(EditorController, ObjectBase)
{
	PRI(Brush, GETREF, Brush)
	PRI(Grid, GETREF, Grid)
	PRI(Atlas, GETREF, Atlas)
	PRI(SelectedTiles, GETREF, std::list<GameObject*>)

public:

	void init();
	void drawGrid() const;
	bool canUseBrush() const;

	GameObject* createTile(const Vector2 &position, const Vector2 &size, Material *material, const Rectangle &region);

	void forEachSelectedTile(TileCallback tileCallback);

	void saveScene();
	void loadScene();
};