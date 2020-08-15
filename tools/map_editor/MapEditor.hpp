#ifndef DE_MAPEDITOR_H
#define DE_MAPEDITOR_H

#include "Script.hpp"
#include "Vector2.hpp"
#include "MapEditorUI.hpp"
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
class ConfigMap;

class MapEditor: public Script {
private:

	class CellData: public DE_Class {
	public: DE_CLASS(CellData, DE_Class);

		GameObject* tile = nullptr;
		Array<GameObject*>* layers = nullptr;

		void addGameObject(GameObject *gameObject, u32 layer);
		void removeGameObject(u32 layer);
		GameObject* get(u32 layer);
	};

	class Brush: public DE_Class {
	private: Array<GameObject*>* mGrid = nullptr;
		Vector2 mOriginAtlasPosition = Vector2(0,0);

		void free();

	public: f32 mDrawTileSize = 100;
		u32 mLastIndex = 0;
		u32 mMaxGridSize = 16;
		Vector2 mGridSize = Vector2(1,1);
		GameObject* mBrushCursor = nullptr;
		MapEditor* mMapEditor = nullptr;

		DE_CLASS(Brush, DE_Class);

		void init(MapEditor* mapEditor);
		void addTile(GameObject* tile, Vector2 atlasPosition);
		GameObject* getTile(u32 i, u32 j);
		void clear();
		void setDrawTileSize(f32 size);
	};



	u32 mGridSize;
	Array<Array<CellData*>*>* mGrid = nullptr;

	Transform* mTransform = nullptr;
	Camera* mCamera = nullptr;
	Transform* mCameraTransform = nullptr;

	bool mCameraControl = true;

public:

	u32 mLayer = 0;
	f32 mZoom = 1;
	GameObject* mPlayer = nullptr;
	GameObject* mSelectedTile = nullptr;
	MapEditorUI mMapEditorUI;
	Material* mMaterial = nullptr;
	f32 mGridTileSize;

	ConfigMap* mConfigMap = nullptr;

	Brush mBrush;

	bool mIsPaintMode = false;

	DE_CLASS(MapEditor, Script);

	void createBrush();
	void resetBrush();

	GameObject* createTile(f32 x, f32 y);
	void createPlayer();
	void destroyPlayer();

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
