#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"
#include "EditorUIElement.hpp"

class GameObject;
class Material;

enum class BrushMode
{
	SELECT,
	PAINT
};

enum class BrushPaintMode
{
	PAINT,
	ERASE
};

class BrushPaintData: public ObjectBase
{
    GENERATE_METADATA(BrushPaintData)
	PUB Rectangle mRegion;
	PUB Material* mMaterial = nullptr;
};

class Brush: public EditorUIElement
{
    GENERATE_METADATA(Brush)
	PRI GameObject* mSelector = nullptr;
	PRI std::vector<GameObject *> mBrushPreview;

	PRI BrushMode mMode;
	PRI BrushPaintMode mPaintMode;

	PRI BrushPaintData mPaintData; GETREF(PaintData)

	PRI u32 mBrushSize = 0; GET(BrushSize)
	PRI u32 mMaxBrushSize = 0;

PRI

	Vector2 getMouseWorldPosition() const;
	void onHold();
	void applyOneTile(const Vector2 &worldPosition);
	void onMouseMoved();
	void createSelector();
	void setSelectorVisibility(bool visible);
	void setBrushPreviewVisibility(bool visible);
	void createBrushPreviewOneTile(const Vector2 &brushPreviewIndex);
	void createBrushPreview();
	void removeBrushPreview();
	void paintTile(const Vector2 &worldPosition, const Vector2 &gridPosition);
	void removeTile(const Vector2 &gridPosition);

PUB

	void init(EditorController* editorController) override;
	void onTileSelectedFromAtlas(GameObject* tile);

	void setModeSelect();
	void setModePaint();
	void setModeErase();
	void setBrushSize(u32 size);
};