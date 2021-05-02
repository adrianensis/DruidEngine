#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "StringsUI.hpp"
#include "Inspector/Inspector.hpp"
#include "Brush/Brush.hpp"
#include "Atlas/Atlas.hpp"
#include "MenuBar/MenuBar.hpp"
#include "Containers/List.hpp"

namespace DE {

class Transform;
class Renderer;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class UITextEditable;
class Texture;
class Material;

class MapEditor;

class MapEditorUI : public ObjectBase{

private:

	// SPRITES
	String mSpritesUIGroup = "sprites";
	List<UIButton*>* mSpriteButtons = nullptr;

	// GRID
	bool mIsGridShow = false;

	// INFO BAR
	UIText* mTextFPS = nullptr;

	MapEditor* mMapEditor;

	MenuBar* mMenuBar;
	Inspector* mInspector;
	Atlas* mAtlas;

public:

	GENERATE_METADATA(MapEditorUI);

	MapEditorUI();
	virtual ~MapEditorUI() override;

	GET(MenuBar);
	GET(Inspector);
	GET(Atlas);

	u32 mUILayer = 3;

	// brush
	Brush mBrush;

	void init(MapEditor *mapEditor);

	void createBrush();
	void resetBrush();

	void createMenuBar();

	void createInfoBar();

	void createAtlas();
	void toggleAtlas();

	void createSpriteFromBrush();

	void createInspector();

	void update();
	void updateInfoBar();

	void updateGridLines();
	void toggleGrid();

	void selectLayer(u32 layer);
};
}

