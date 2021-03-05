#pragma once

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
#include "Config/ConfigMap.hpp"
#include "Scene/Scene.hpp"

#include "EditorEvents/EditorEvents.hpp"
#include "MapEditorUI/MapEditorUI.hpp"

#include "MapElement/MapElement.hpp"

namespace DE {

	class MapEditor;

class Grid: public DE_Class {
private:

	class CellData: public DE_Class {
	public:
		DE_CLASS_BODY(CellData)

		/*
			TODO : Migrate this to MapElement.
			loadMapIntoGrid should load MapElements.
		*/
		Array<MapElement*> *layers = nullptr;

		void set(MapElement* gameObject, u32 layer);
		void remove(u32 layer);
		MapElement* get(u32 layer);
	};

	DE_M(Grid, Array<Array<CellData*>*>*)
	DE_M(MapEditor, MapEditor*)
	DE_M(Scene, Scene*);

	DE_M_GET(GridSize, u32)
	DE_M_GET(TileSize, f32)
	DE_M_GET(SelectedMapElements, List<MapElement*>*)

	void draw(CellData *cellData, const Vector3 &worldPosition);
	void remove(CellData *cellData, u32 layer);
	void select(CellData *cellData, u32 layer, bool multi);

public:
	DE_CLASS_BODY(Grid)

	void click(const Vector3 &clampedPosition, GameObject* brushTile);

	MapElement* getFirstSelectedTile();

	void init(MapEditor* mapEditor, u32 gridSize, f32 tileSize);

	void loadMapIntoGrid(const List<GameObject*>* gameObjects);

	void forEachSelectedTile(std::function<void(MapElement*)> callback);
};

}

