#pragma once

#include "Core/ObjectBase.hpp"
#include "EditorUI/Brush.hpp"
#include "EditorUI/Grid.hpp"
#include "EditorUI/Atlas.hpp"
#include "EditorUI/MenuBar.hpp"
#include "EditorUI/InfoBar.hpp"
#include "EditorUI/ToolsBar.hpp"
#include "EditorUI/LayersBar.hpp"
#include "EditorUI/EditorStyles.hpp"

#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"
#include <list>
#include <vector>

class Material;
class GameObject;
class Camera;

using TileCallback = std::function<void(GameObject* tile)>;

CLASS(EditorController, ObjectBase)
{
	PRI(Brush, GETREF, Brush)
	PRI(Atlas, GETREF, Atlas)
	PRI(MenuBar, GETREF, MenuBar)
	PRI(InfoBar, GETREF, InfoBar)
	PRI(ToolsBar, GETREF, ToolsBar)
	PRI(LayersBar, GETREF, LayersBar)
	PRI(SelectedTiles, GETREF, std::list<GameObject*>)

	PRI(Layer, GET_SET, u32)
	PRI(Grids, NONE, std::vector<Grid>)
	PRI(DrawGrid, GET_SET, bool)

	PRI(Camera, GET, Camera*)
	PRI(CameraSpeed, NONE, f32)
	PRI(CameraDragStarted, NONE, bool)
	PRI(CameraDragLastPosition, NONE, Vector3)

public:

	void init();
	void update();
	void drawGrid();
	bool canUseBrush() const;

	Grid& getGrid();

	GameObject* createTile(const Vector2 &position, const Vector2 &size, Material *material, const Rectangle &region);

	void forEachSelectedTile(TileCallback tileCallback);

	void saveScene();
	void loadScene();

	void moveCameraKeys();
	void moveCameraMouse();
	void releaseCameraMouse();
	void zoom(f32 scroll);
	void handleHoldKeys();
	void handlePressedKeys();
};