#pragma once

#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

class GameObject;
class UIButton;
template<class T> class Array;
class MapElementData;

class MapEditor;
class Material;

class Brush: public DE_Class {
private:

	Array<GameObject*>* mGrid = nullptr;
	Vector2 mOriginAtlasPosition = Vector2(0,0);
	DE_M_GET_SET(IsPaintMode, bool);

	void addTile(GameObject *tile, const Vector2& atlasPosition);

public:

	Vector2 mDrawTileSize = Vector2(1,1);
	u32 mLastIndex = 0;
	u32 mBrushMaxGridSize = 16;
	Vector2 mBrushGridSize = Vector2(1,1);
	GameObject* mBrushCursor = nullptr;
	MapEditor* mMapEditor = nullptr;

	DE_M(MapElementData, MapElementData*);

	DE_CLASS_BODY(Brush)

	void init(MapEditor* mapEditor);
	void setMaterial(Material* material);
	void setMaterialRegion(const Vector2& regionPosition, const Vector2& regionSize);
	void clickTile(GameObject *tile, const Vector2& atlasPosition);
	GameObject* getTile(u32 i, u32 j);
	void free();
	void update();
	void clear();
	void setDrawTileSize(const Vector2& size);
};

}

