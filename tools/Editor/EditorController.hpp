#pragma once

#include "Core/Core.hpp"
#include "EditorUI/Brush.hpp"
#include "EditorUI/Grid.hpp"
#include "EditorUI/MenuBar.hpp"
#include "EditorUI/InfoBar.hpp"
#include "EditorUI/ToolsBar.hpp"
#include "EditorUI/LayersBar.hpp"
#include "EditorUI/Sprites.hpp"
#include "EditorUI/EditorStyles.hpp"
#include "Config/ConfigObject.hpp"

#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"

class Material;
class GameObject;
class Camera;

using TileCallback = SFun<void(GameObject* tile)>;

CLASS(EditorController, ObjectBase)
{
	PRI(EditorConfig, GETREF_CONST, ConfigObject)

	PRI(Brush, GETREF, Brush)
	PRI(MenuBar, GETREF, MenuBar)
	PRI(InfoBar, GETREF, InfoBar)
	PRI(ToolsBar, GETREF, ToolsBar)
	PRI(LayersBar, GETREF, LayersBar)
	PRI(Sprites, GETREF, Sprites)

	PRI(SelectedTiles, GETREF, SLst<GameObject*>)

	PRI(Layer, GET_SET, u32)
	PRI(Grids, NONE, SVec<Grid>)
	PRI(DrawGrid, GET_SET, bool)

	PRI(Camera, GET, Camera*)
	PRI(CameraSpeed, NONE, f32)
	PRI(CameraDragStarted, NONE, bool)
	PRI(CameraDragLastPosition, NONE, Vector3)

public:

	void init();
	void update();
	void drawGrid();
	bool canUseBrush() CNS;

	Grid& getGrid();

	GameObject* createTile(CNS Vector2 &position, CNS Vector2 &size, Material *material, CNS Rectangle &region);

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