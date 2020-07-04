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
	public:DE_CLASS(CellData, DE_Class)
		;

		GameObject *tile = nullptr;
		Array<GameObject*> *layers = nullptr;

		void addGameObject(GameObject *gameObject, u32 layer);
		void removeGameObject(u32 layer);
		GameObject* get(u32 layer);
	};

	u32 mGridSize;
	Array<Array<CellData*>*> *mGrid;

	Transform *mTransform;
	Camera *mCamera;
	Transform *mCameraTransform;

	GameObject *mPlayer;
	GameObject *mTile;

	Material *mMaterial;

	bool mTestCreated;
	u32 mTileIndex;
	f32 mTileSize;

	bool mCameraControl;

	class InfoBarUI: public DE_Class {
	public:DE_CLASS(InfoBarUI, DE_Class)
		;

		MapEditor *mMapEditor;

		UIText *mTextLayer = nullptr;
		UIText *mTextTile = nullptr;
		UIText *mTextZoom = nullptr;
		UIText *mTextBrush = nullptr;

		const std::string mStrLayer = "Layer";
		const std::string mStrTile = "Tile";
		const std::string mStrZoom = "Zoom";
		const std::string mStrBrush = "Brush";
		Vector2 mTextSize = Vector2(0.04f, 0.04f);

		void init(MapEditor *mapEditor);

		void createUI();

		void updateUI();
	};

	InfoBarUI *mInfoBarUI;

	List<UIButton*> *mButtons;

public:

	u32 mBrushSize;
	u32 mLayer;
	f32 mZoom;
	u32 mUILayer = 0;
	GameObject *mBrush;

	DE_CLASS(MapEditor, Script)
	;

	void createBrush();
	void createAtlas();
	void createTile(f32 x, f32 y);
	void createPlayer();

	void click(const Vector3 &clampedPosition);
	void drawTile(CellData *cellData, const Vector3 &worldPosition);
	void removeTile(CellData *cellData);

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
