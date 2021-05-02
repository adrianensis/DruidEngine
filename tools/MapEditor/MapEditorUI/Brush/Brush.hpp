#pragma once

#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

template<class T> class Array;
class GameObject;
class UIButton;
class MapElementData;
class MapEditor;
class Material;

class Brush : public ObjectBase {
private:

	Array<GameObject*>* mGrid;
	Vector2 mOriginAtlasPosition;
	bool mIsPaintMode;
	Vector2 mDrawTileSize;
	u32 mLastIndex;
	u32 mBrushMaxGridSize;
	Vector2 mBrushGridSize;
	GameObject* mBrushCursor;
	MapEditor* mMapEditor;
	u32 mBrushSize;

	MapElementData* mMapElementData;

	void addTile(GameObject *tile, const Vector2& atlasPosition);

public:

	Brush();
	virtual ~Brush() override;

	void init(MapEditor* mapEditor);
	void setMaterial(Material* material);
	void setMaterialRegion(const Vector2& regionPosition, const Vector2& regionSize);
	void clickTile(GameObject *tile, const Vector2& atlasPosition);
	GameObject* getTile(u32 i, u32 j);
	void free();
	void update();
	void clear();
	void setDrawTileSize(const Vector2& size);
	bool isGridSingleTile() const { return mBrushGridSize.x == 1 && mBrushGridSize.y == 1; }
	void setBrushSize(i32 newSize);

	GENERATE_METADATA(Brush);
	
	GET_SET(IsPaintMode);
	GET(DrawTileSize);
	GET(LastIndex);
	GET(BrushMaxGridSize);
	GET(BrushGridSize);
	GET_SET(BrushCursor);
	GET(MapEditor);
	GET(BrushSize);
	GET_SET(MapElementData);
};
}

