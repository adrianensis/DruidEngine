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

	class MapEditorUI {

	public:

		class StringsUI {
		public:
			static const std::string smLayer;
			static const std::string smTile;
			static const std::string smZoom;
			static const std::string smBrush;
			static const std::string smSave;
			static const std::string smCollider;
			static const std::string smPlay;
			static const std::string smInspectorTileX;
			static const std::string smInspectorTileY;
			static const std::string smInspectorTileCollider;
			static const std::string smInspectorTileTag;
		};

		MapEditor* mMapEditor;
		u32 mUILayer = 3;

		UIText* mTextLayer = nullptr;
		UIText* mTextTile = nullptr;
		UIText* mTextZoom = nullptr;
		UIText* mTextBrush = nullptr;

		UIText* mTextInspectorTag = nullptr;
		UIText* mTextInspectorX = nullptr;
		UIText* mTextInspectorY = nullptr;
		UIText* mTextInspectorCollider = nullptr;

		Vector2 mTextSize = Vector2(0.045f, 0.045f);

		void init(MapEditor *mapEditor);

		void createUI();
		void createMenuBarUI();
		void createInspectorUI();
		void createAtlasUI();

		void updateUI();
	};

	MapEditorUI mInfoBarUI;

public:

	u32 mBrushSize;
	u32 mLayer;
	f32 mZoom;
	GameObject* mPlayer;
	GameObject* mTile;

	GameObject* mBrush;
	bool mIsPaintMode;

	DE_CLASS(MapEditor, Script);

	void createBrush();

	GameObject* createTile(f32 x, f32 y);
	void createPlayer();

	void click(const Vector3 &clampedPosition);
	void drawTile(CellData *cellData, const Vector3 &worldPosition);
	void removeTile(CellData *cellData);
	void selectTile(CellData *cellData);

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
