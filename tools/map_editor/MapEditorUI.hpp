#ifndef DE_MAPEDITORUI_H
#define DE_MAPEDITORUI_H

#include "Script.hpp"
#include "Vector2.hpp"
#include <string>
#include <functional>

namespace DE {

class Transform;
class Renderer;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIElement;
class UIButton;
class UIText;
class UITextEditable;
class Texture;
class Material;
template<class K, class V> class HashMap;
template<class T> class Array;
template<class T> class List;

class MapEditor;

class Brush: public DE_Class {
private:

	Array<GameObject*>* mGrid = nullptr;
	Vector2 mOriginAtlasPosition = Vector2(0,0);

public:

	Vector2 mDrawTileSize = Vector2(1,1);
	u32 mLastIndex = 0;
	u32 mBrushMaxGridSize = 16;
	Vector2 mBrushGridSize = Vector2(1,1);
	GameObject* mBrushCursor = nullptr;
	MapEditor* mMapEditor = nullptr;

	DE_CLASS(Brush, DE_Class);

	void init(MapEditor* mapEditor);
	void clickTile(UIButton* tileButton, Vector2 atlasPosition);
	GameObject* getTile(u32 i, u32 j);
	void free();
	void clear();
	void setDrawTileSize(const Vector2& size);
};

// Vector2 Text Editable
class TextEditableVector2 {
public:
	UITextEditable* TextEditableX = nullptr;
	UITextEditable* TextEditableY = nullptr;
};

/**
 * @brief
 */
class MapEditorUI : public DE_Class{

private:

	// STRINGS
	class StringsUI {
	public:
		std::string Tile = "Tile";
		std::string Zoom = "Zoom";
		std::string Brush = "Brush";
		std::string Save = "Save";
		std::string Collider = "Colliders";
		std::string Atlas = "Atlas";
		std::string Play = "Play";
		std::string AddSprite = "Add Sprite";
		std::string InspectorTileX = "x:";
		std::string InspectorTileY = "y:";
		std::string InspectorTileCollider = "collider:";
		std::string InspectorTileTag = "tag:";
		std::string InspectorSize = "size:";
		std::string InspectorSizeCollider = "collider size:";
		std::string InspectorPosCollider = "collider offset:";
		std::string Grid = "Grid";
	};

	const StringsUI mStringsUI;

	// ATLAS
	std::string mAtlasSelectorUIGroup = "atlasSelector";
	std::string mAtlasUIGroup = "atlas";
	Array<UIButton*>* mAtlasButtons = nullptr;
	bool mIsAtlasShow = true;
	GameObject* mAtlasBackground = nullptr;

	// SPRITES
	std::string mSpritesUIGroup = "sprites";
	Array<UIButton*>* mSpriteButtons = nullptr;

	// GRID
	bool mIsGridShow = false;

	// LAYERS
	Array<UIButton*>* mLayerButtons = nullptr;

	// INSPECTOR
	UIText* mTextBoxTag = nullptr;
	UIText* mTextInspectorX = nullptr;
	UIText* mTextInspectorY = nullptr;
	UIText* mTextBoxSizeX = nullptr;
	UIText* mTextBoxSizeY = nullptr;
	UIText* mTextBoxColliderPosX = nullptr;
	UIText* mTextBoxColliderPosY = nullptr;
	UIText* mTextBoxColliderSizeX = nullptr;
	UIText* mTextBoxColliderSizeY = nullptr;
	UIButton* mButtonInspectorCollider = nullptr;

	MapEditor* mMapEditor;

public:

	DE_CLASS(MapEditorUI, DE_Class);

	u32 mUILayer = 3;

	// brush
	Brush mBrush;

	void init(MapEditor *mapEditor);

	void createBrush();
	void resetBrush();

	void createPanel(const Vector2& position, const Vector2& size);

	void addMenuEntry(const std::string& title, std::function<void()> onPressedCallback);
	void createMenuBar();

	UIText* createInspectorLabel(const std::string& text);
	UITextEditable* createInspectorTextBoxSimple(const std::string& text, std::function<void(UIElement* uiElement)> onTextChangedCallback);
	UITextEditable* createInspectorTextBoxLabeled(const std::string& textLabel, const std::string& text, std::function<void(UIElement* uiElement)> onTextChangedCallback);
	TextEditableVector2 createInspectorTextBoxVector2(const std::string& textLabel, std::function<void(UIElement* uiElement)> onTextChangedCallbackX, std::function<void(UIElement* uiElement)> onTextChangedCallbackY);
	void createInspector();

	void createAtlasSelector();
	void createAtlas(u32 index, Material* material);
	void toggleAtlas();

	void createSpriteFromBrush();

	void createLayersBar();

	void update();
	void updateInspector();
	void updateInspectorOnSelectTile();

	void updateGridLines();
	void toggleGrid();

	void selectLayer(u32 layer);
};

} /* namespace DE */

#endif /* DE_MAPEDITORUI_H */
