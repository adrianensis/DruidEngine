#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "StringsUI.hpp"
#include "Inspector/Inspector.hpp"
#include "Brush/Brush.hpp"
#include "Atlas/Atlas.hpp"
#include "MenuBar/MenuBar.hpp"

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
template<class K, class V> class HashMap;
template<class T> class Array;
template<class T> class List;

class MapEditor;

class MapEditorUI : public DE_Class{

private:

	const StringsUI mStringsUI;

	MenuBar* mMenuBar;
	Inspector* mInspector = nullptr;
	Atlas* mAtlas = nullptr;

	// SPRITES
	std::string mSpritesUIGroup = "sprites";
	Array<UIButton*>* mSpriteButtons = nullptr;

	// GRID
	bool mIsGridShow = false;

	// INFO BAR
	UIText* mTextFPS = nullptr;

	MapEditor* mMapEditor;

public:

	DE_CLASS(MapEditorUI)

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

