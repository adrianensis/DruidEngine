#ifndef DE_GRID_H
#define DE_GRID_H

#include "DE_Class.hpp"
#include "Vector2.hpp"
#include "MapEditorUI.hpp"
#include <string>

#include "EditorEvents.hpp"

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
class Scene;


class Grid: public DE_Class {
private:

	class CellData: public DE_Class {
	public: DE_CLASS(CellData, DE_Class);

		GameObject* tile = nullptr;
		Array<GameObject*>* layers = nullptr;

		void addGameObject(GameObject *gameObject, u32 layer);
		void removeGameObject(u32 layer);
		GameObject* get(u32 layer);
	};

	u32 mGridSize;
	Array<Array<CellData*>*>* mGrid = nullptr;

	Scene* mScene;
	f32 mGridTileSize;

public:

	GameObject* mSelectedTile = nullptr;
	bool mIsPaintMode = false;

	DE_CLASS(Grid, DE_Class);

	GameObject* createTile(f32 x, f32 y, f32 size, Material* material, u32 layer);

	void click(const Vector3 &clampedPosition, GameObject* brush, u32 tileSize, u32 layer);
	bool isSameTile(GameObject* tileA, GameObject* tileB);
	void drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brush, u32 size, u32 layer);
	void removeTile(CellData *cellData, u32 layer);
	void selectTile(CellData *cellData, u32 layer);

	void init(Scene* scene, u32 gridSize, f32 gridTileSize);

	u32 getGridSize() const { return mGridSize; }
	f32 getGridTileSize() const { return mGridTileSize; }

	void loadMapIntoGrid(const List<GameObject*>* gameObjects);
};

} /* namespace DE */

#endif /* DE_GRID_H */
