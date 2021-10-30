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
	PRI_M(ConfigObject, EditorConfig, GETREF_CONST)

	PRI_M(Brush, Brush, GETREF)
	PRI_M(MenuBar, MenuBar, GETREF)
	PRI_M(InfoBar, InfoBar, GETREF)
	PRI_M(ToolsBar, ToolsBar, GETREF)
	PRI_M(LayersBar, LayersBar, GETREF)
	PRI_M(Sprites, Sprites, GETREF)

	PRI_M(SLst(GameObject*), SelectedTiles, GETREF)

	PRI_M(u32, Layer, GET_SET)
	PRI_M(SVec(Grid), Grids, NONE)
	PRI_M(bool, DrawGrid, GET_SET)

	PRI_M(Camera*, Camera, GET)
	PRI_M(f32, CameraSpeed, NONE)
	PRI_M(bool, CameraDragStarted, NONE)
	PRI_M(Vector3, CameraDragLastPosition, NONE)

PUB

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