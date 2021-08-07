#pragma once

#include "Core/ObjectBase.hpp"
#include "Brush.hpp"
#include "Grid.hpp"
#include "Atlas.hpp"
#include "MenuBar.hpp"
#include "ToolsBar.hpp"
#include "EditorStyles.hpp"

#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"
#include <list>
#include <vector>

class Material;
class GameObject;

using TileCallback = std::function<void(GameObject* tile)>;

CLASS(EditorController, ObjectBase)
{
	PRI(Brush, GETREF, Brush)
	PRI(Atlas, GETREF, Atlas)
	PRI(MenuBar, GETREF, MenuBar)
	PRI(ToolsBar, GETREF, ToolsBar)
	PRI(SelectedTiles, GETREF, std::list<GameObject*>)

	PRI(Layer, GET, u32)
	PRI(Grids, NONE, std::vector<Grid>)

public:

	void init();
	void drawGrid();
	bool canUseBrush() const;

	Grid& getGrid();

	GameObject* createTile(const Vector2 &position, const Vector2 &size, Material *material, const Rectangle &region);

	void forEachSelectedTile(TileCallback tileCallback);

	void saveScene();
	void loadScene();
};