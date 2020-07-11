#ifndef DE_MAPEDITOR_H
#define DE_MAPEDITOR_H

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

class MapEditor: public Script {
private:

	class CellData: public DE_Class {
	public:
		DE_CLASS(CellData, DE_Class);

		GameObject* tile = nullptr;
		Array<GameObject*>* layers = nullptr;

		void addGameObject(GameObject *gameObject, u32 layer);
		void removeGameObject(u32 layer);
		GameObject* get(u32 layer);
	};

	class Brush: public DE_Class {
	private:
		void free();

	public:
		DE_CLASS(Brush, DE_Class);

		void init();

		u32 mMaxSize = 20;
		Vector2 mSize = Vector2(1,1);
		Vector2 mOriginAtlasPosition = Vector2(0,0);
		u32 mLastIndex = 0;
		Array<GameObject*>* mGrid = nullptr;

		void addTile(GameObject* tile, Vector2 atlasPosition);
		GameObject* getTile(u32 i, u32 j);
		void clear();
	};

	u32 mGridSize;
	Array<Array<CellData*>*>* mGrid;

	Transform* mTransform;
	Camera* mCamera;
	Transform* mCameraTransform;

	Material* mMaterial;

	bool mTestCreated;
	u32 mTileIndex;
	f32 mTileSize;

	bool mCameraControl;

	class MapEditorUI : public DE_Class{

	private:

		class StringsUI {
		public:
			static const std::string smLayer;
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

		UIButton* mButtonInspectorCollider = nullptr;

		Vector2 mTextSize = Vector2(0.045f, 0.045f);

	public:

		DE_CLASS(MapEditorUI, DE_Class);

		u32 mUILayer = 3;

		void init(MapEditor *mapEditor);

		void createUI();
		void createMenuBarUI();
		void createInspectorUI();
		void createAtlasUI();

		void updateUI();
		void updateInspectorUI();

		void toggleAtlasUI();
	};

	MapEditorUI mMapEditorUI;

public:

	u32 mBrushSize;
	u32 mLayer;
	f32 mZoom;
	GameObject* mPlayer;
	GameObject* mTile;

	Brush mBrush;

	GameObject* mBrushCursor;
	bool mIsPaintMode;

	DE_CLASS(MapEditor, Script);

	void createBrush();
	void resetBrush();

	GameObject* createTile(f32 x, f32 y);
	void createPlayer();

	void click(const Vector3 &clampedPosition, GameObject* brush);
	void drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brush);
	void removeTile(CellData *cellData);
	void selectTile(CellData *cellData);
	void addColliderToTile(GameObject* tile);

	void loadMapIntoGrid();

	void cameraZoom();
	void processMovement();
	void switchCameraControl();

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};

} /* namespace DE */

#endif /* DE_MAPEDITOR_H */
