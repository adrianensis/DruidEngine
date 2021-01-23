#ifndef DE_GRID_H
#define DE_GRID_H

#include "Core/DE_Class.hpp"
#include "Maths/Vector2.hpp"
#include <string>
#include "Core/Functor.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Renderer.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Scene/GameObject.hpp"
#include "Graphics/Camera.hpp"
#include "UI/UIText.hpp"
#include "UI/UIButton.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"
#include "Containers/List.hpp"
#include "Core/ConfigMap.hpp"
#include "Scene/Scene.hpp"

#include "EditorEvents/EditorEvents.hpp"
#include "MapEditorUI/MapEditorUI.hpp"

#include "MapEditorUI/MapElement/MapElement.hpp"
namespace DE {

class Grid: public DE_Class {
private:

	class CellData: public DE_Class {
	public:
		DE_CLASS(CellData)

		/*
			TODO : Migrate this to MapElement.

			loadMapIntoGrid should load MapElements.
		*/
		Array<GameObject*> *layers = nullptr;

		void addGameObject(GameObject* gameObject, u32 layer);
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

	DE_CLASS(Grid)

	void click(const Vector3 &clampedPosition, GameObject* brush, const Vector2& tileSize, u32 layer);
	bool isSameTile(GameObject* tileA, GameObject* tileB);
	void drawTile(CellData *cellData, const Vector3 &worldPosition, GameObject* brushTile, const Vector2 &size, u32 layer);
	void removeTile(CellData *cellData, u32 layer);
	void selectTile(CellData *cellData, u32 layer, bool multi);

	GameObject* getFirstSelectedTile();

	void init(Scene* scene, u32 gridSize, f32 gridTileSize);

	void loadMapIntoGrid(const List<GameObject*>* gameObjects);

	void forEachSelectedTile(std::function<void(GameObject*)> callback);
};

} /* namespace DE */

#endif /* DE_GRID_H */
