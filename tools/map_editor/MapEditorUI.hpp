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
		static const std::string smInspectorTileX;
		static const std::string smInspectorTileY;
		static const std::string smInspectorTileCollider;
		static const std::string smInspectorTileTag;
		static const std::string smInspectorSize;
		static const std::string smInspectorSizeCollider;
		static const std::string smGrid;
	};

	Array<UIButton*>* mAtlasButtons = nullptr;
	bool mIsAtlasShow = true;
	GameObject* mAtlasBackground = nullptr;

	MapEditor* mMapEditor;

	UIText* mTextLayer = nullptr;
	UIText* mTextTile = nullptr;
	UIText* mTextZoom = nullptr;
	UIText* mTextBrush = nullptr;

	UIText* mTextInspectorTag = nullptr;
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

	bool mIsGridShow = false;

public:

	DE_CLASS(MapEditorUI, DE_Class);

	u32 mUILayer = 3;

	void init(MapEditor *mapEditor);

	void createMenuBar();

	void createInspector();

	void createAtlas();
	void toggleAtlas();

	void createSprites();

	void update();
	void updateInspector();
	void updateInspectorOnSelectTile();

	void updateGridLines();
	void toggleGrid();


};

} /* namespace DE */

#endif /* DE_MAPEDITORUI_H */
