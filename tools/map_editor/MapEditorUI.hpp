#ifndef DE_MAPEDITORUI_H
#define DE_MAPEDITORUI_H

#include "Script.hpp"
#include "Vector2.hpp"
#include <string>

namespace DE {

class Transform;
class Renderer;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Texture;
class Material;
template<class K, class V> class HashMap;
template<class T> class Array;
template<class T> class List;

class MapEditor;

class Brush: public DE_Class {
private: Array<GameObject*>* mGrid = nullptr;
	Vector2 mOriginAtlasPosition = Vector2(0,0);



public:

	f32 mDrawTileSize = 100;
	u32 mLastIndex = 0;
	u32 mBrushMaxGridSize = 16;
	Vector2 mBrushGridSize = Vector2(1,1);
	GameObject* mBrushCursor = nullptr;
	MapEditor* mMapEditor = nullptr;

	DE_CLASS(Brush, DE_Class);

	void init(MapEditor* mapEditor);
	void addTile(GameObject* tile, Vector2 atlasPosition);
	GameObject* getTile(u32 i, u32 j);
	void free();
	void clear();
	void setDrawTileSize(f32 size);
};


class MapEditorUI : public DE_Class{

private:

	class StringsUI {
	public: static const std::string smLayer;
		static const std::string smTile;
		static const std::string smZoom;
		static const std::string smBrush;
		static const std::string smSave;
		static const std::string smCollider;
		static const std::string smAtlas;
		static const std::string smPlay;
		static const std::string smCreateSprite;
		static const std::string smInspectorTileX;
		static const std::string smInspectorTileY;
		static const std::string smInspectorTileCollider;
		static const std::string smInspectorTileTag;
		static const std::string smInspectorSize;
		static const std::string smInspectorSizeCollider;
		static const std::string smGrid;
	};

	std::string mAtlasSelectorUIGroup = "atlasSelector";
	std::string mAtlasUIGroup = "atlas";
	std::string mSpritesUIGroup = "sprites";

	Array<UIButton*>* mAtlasButtons = nullptr;
	bool mIsAtlasShow = true;
	GameObject* mAtlasBackground = nullptr;

	MapEditor* mMapEditor;

	UIText* mTextTile = nullptr;
	UIText* mTextZoom = nullptr;
	UIText* mTextBrush = nullptr;

	UIText* mTextInspectorTag = nullptr;
	UIText* mTextBoxTag = nullptr;
	UIText* mTextInspectorX = nullptr;
	UIText* mTextInspectorY = nullptr;
	UIText* mTextInspectorCollider = nullptr;
	UIText* mTextTileSize = nullptr;
	UIText* mTextBoxSizeX = nullptr;
	UIText* mTextBoxSizeY = nullptr;
	UIText* mTextColliderSize = nullptr;
	UIText* mTextBoxColliderSizeX = nullptr;
	UIText* mTextBoxColliderSizeY = nullptr;

	UIButton* mButtonInspectorCollider = nullptr;

	Vector2 mTextSize = Vector2(0.04f / 1.5f, 0.04f);

	Array<UIButton*>* mSpriteButtons = nullptr;
	Array<UIButton*>* mLayerButtons = nullptr;

	bool mIsGridShow = false;

public:

	DE_CLASS(MapEditorUI, DE_Class);

	u32 mUILayer = 3;

	// brush
	Brush mBrush;

	void init(MapEditor *mapEditor);

	void createBrush();
	void resetBrush();

	void createMenuBar();

	void createInspector();

	void createAtlasSelector();
	void createAtlas(u32 index, Material* material);
	void toggleAtlas();

	void createSprites();

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
