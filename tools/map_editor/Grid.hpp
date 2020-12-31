#ifndef DE_GRID_H
#define DE_GRID_H

#include "DE_Class.hpp"
#include "Vector2.hpp"
#include "MapEditorUI.hpp"
#include <string>
#include "Functor.hpp"
#include "Transform.hpp"
#include "Renderer.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "HashMap.hpp"
#include "Array.hpp"
#include "List.hpp"
#include "ConfigMap.hpp"
#include "Scene.hpp"

#include "EditorEvents.hpp"

namespace DE {

class Grid: public DE_Class {
private:

	class CellData: public DE_Class {
	public:
		DE_CLASS(CellData, DE_Class)

		Array<GameObject*>* layers = nullptr;

		void addGameObject(GameObject *gameObject, u32 layer);
		void removeGameObject(u32 layer);
		GameObject* get(u32 layer);
	};

	DE_M(Grid, Array<Array<CellData*>*>*)
	DE_M(Scene, Scene*);

	DE_M_GET(GridSize, u32)
	DE_M_GET(GridTileSize, f32)
	DE_M_GET(SelectedTiles, List<GameObject*>*)

public:
	DE_M(IsPaintMode, bool);

	DE_CLASS(Grid, DE_Class)

	GameObject* createTile(f32 x, f32 y, const Vector2& size, Material* material, u32 layer);

	void click(const Vector3 &clampedPosition, GameObject* brush, const Vector2& tileSize, u32 layer);
	bool isSameTile(GameObject* tileA, GameObject* tileB);
	void drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brush, const Vector2& size, u32 layer);
	void removeTile(CellData *cellData, u32 layer);
	void selectTile(CellData *cellData, u32 layer, bool multi);

	GameObject* getFirstSelectedTile();

	void init(Scene* scene, u32 gridSize, f32 gridTileSize);

	void loadMapIntoGrid(const List<GameObject*>* gameObjects);

	void forEachSelectedTile(std::function<void(GameObject*)> callback);
};

} /* namespace DE */

#endif /* DE_GRID_H */
