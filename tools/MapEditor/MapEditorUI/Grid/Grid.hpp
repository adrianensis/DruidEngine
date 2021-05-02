#pragma once

#include "Core/ObjectBase.hpp"
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
#include "Config/ConfigMap.hpp"
#include "Scene/Scene.hpp"

#include "EditorEvents/EditorEvents.hpp"
#include "MapEditorUI/MapEditorUI.hpp"

#include "Scene/MapElement/MapElement.hpp"

namespace DE {

	class MapEditor;

class Grid : public ObjectBase {
private:

	class CellData : public ObjectBase {
	public:
		GENERATE_METADATA(CellData);

	CellData();
	virtual ~CellData() override;

		/*
			TODO : Migrate this to MapElement.
			loadMapIntoGrid should load MapElements.
		*/
		Array<MapElement*> *layers = nullptr;

		void set(MapElement* gameObject, u32 layer);
		void remove(u32 layer);
		MapElement* get(u32 layer);
	};

	 Array<Array<CellData*>*>* mGrid;
	 MapEditor* mMapEditor;
	 Scene* mScene;

	 u32 mGridSize;
	 f32 mTileSize;
	 List<MapElement*>* mSelectedMapElements;

	void draw(GameObject* brushTile, CellData *cellData, const Vector3 &worldPosition);
	void remove(CellData *cellData, u32 layer);
	void select(CellData *cellData, u32 layer, bool multi);

public:
	GENERATE_METADATA(Grid);

	Grid();
	virtual ~Grid() override;;

	GET(GridSize);
	GET(TileSize);
	GET(SelectedMapElements);

	void click(const Vector3 &clampedPosition, GameObject* brushTile);

	MapElement* getFirstSelectedTile();

	void init(MapEditor* mapEditor, u32 gridSize, f32 tileSize);

	void update();

	void loadMapIntoGrid(const List<GameObject*>* gameObjects);

	void forEachSelectedTile(std::function<void(MapElement*)> callback);
};
}

