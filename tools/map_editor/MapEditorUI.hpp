#ifndef DE_MAPEDITORUI_H
#define DE_MAPEDITORUI_H

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"

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

	DE_CLASS(Brush)

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
		std::string BoolFalse = "[ ]";
		std::string BoolTrue = "[X]";
		std::string FPS = "FPS:";
		std::string Tile = "Tile";
		std::string Zoom = "Zoom";
		std::string Brush = "Brush";
		std::string Save = "Save";
		std::string Collider = "Colliders";
		std::string Atlas = "Atlas";
		std::string Play = "Play";
		std::string AddSprite = "Add Sprite";
		std::string Inspector = "INSPECTOR";
		std::string InspectorTilePosition = "pos";
		std::string InspectorTileX = "x";
		std::string InspectorTileY = "y";
		std::string InspectorTileCollider = "collider";
		std::string InspectorTileTag = "tag";
		std::string InspectorSize = "size";
		std::string InspectorSizeCollider = "size";
		std::string InspectorPosCollider = "offset";
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
	UITextEditable* mTextBoxSizeX = nullptr;
	UITextEditable* mTextBoxSizeY = nullptr;
	UITextEditable* mTextBoxColliderPosX = nullptr;
	UITextEditable* mTextBoxColliderPosY = nullptr;
	UITextEditable* mTextBoxColliderSizeX = nullptr;
	UITextEditable* mTextBoxColliderSizeY = nullptr;
	UIButton* mButtonInspectorCollider = nullptr;

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

	UIElement* createPanel(const Vector2& position, const Vector2& size);

	void addMenuEntry(const std::string& title, UIElementCallback onPressedCallback);
	void createMenuBar();

	UIText* createInspectorLabel(const std::string& text);
	UITextEditable* createInspectorTextBoxSimple(const std::string& text, UIElementCallback onTextChangedCallback);
	UITextEditable* createInspectorTextBoxLabeled(const std::string& textLabel, const std::string& text, UIElementCallback onTextChangedCallback);
	UIButton* createInspectorBoolean(const std::string& textLabel, UIElementCallback onPressedCallback);
	TextEditableVector2 createInspectorTextBoxVector2(const std::string& textLabel, UIElementCallback onTextChangedCallbackX, UIElementCallback onTextChangedCallbackY);

	void setTileScaleFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex);
	void setTileColliderPositionFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex);
	void setTileColliderScaleFromInspector(GameObject* tile, const std::string& stringValue, u32 vectorIndex);

	void createInspector();

	void createInfoBar();

	void createAtlasSelector();
	void createAtlas(u32 index, Material* material);
	void toggleAtlas();

	void createSpriteFromBrush();

	void createLayersBar();

	void update();
	void updateInspector();
	void updateInfoBar();
	void updateInspectorOnSelectTile();

	void updateGridLines();
	void toggleGrid();

	void selectLayer(u32 layer);
};

} /* namespace DE */

#endif /* DE_MAPEDITORUI_H */
