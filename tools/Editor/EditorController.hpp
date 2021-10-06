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

using TileCallback = SFun(void(GameObject* tile));

CLASS(EditorController, ObjectBase)
{
	PRI(ConfigObject, EditorConfig, GETREF_CONST)

	PRI(Brush, Brush, GETREF)
	PRI(MenuBar, MenuBar, GETREF)
	PRI(InfoBar, InfoBar, GETREF)
	PRI(ToolsBar, ToolsBar, GETREF)
	PRI(LayersBar, LayersBar, GETREF)
	PRI(Sprites, Sprites, GETREF)

	PRI(SLst(GameObject*), SelectedTiles, GETREF)

	PRI(u32, Layer, GET_SET)
	PRI(SVec(Grid), Grids, NONE)
	PRI(bool, DrawGrid, GET_SET)

	PRI(Camera*, Camera, GET)
	PRI(f32, CameraSpeed, NONE)
	PRI(bool, CameraDragStarted, NONE)
	PRI(Vector3, CameraDragLastPosition, NONE)

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