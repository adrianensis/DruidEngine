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

CLASS(BrushPaintData, ObjectBase)
{
	PUB_M(Rectangle, Region, NONE)
	PUB_M(Material *, Material, NONE)
};

CLASS(Brush, EditorUIElement)
{
	PRI_M(GameObject *, Selector, NONE)
	PRI_M(SVec(GameObject *), BrushPreview, NONE)

	PRI_M(BrushMode, Mode, NONE)
	PRI_M(BrushPaintMode, PaintMode, NONE)

	PRI_M(BrushPaintData, PaintData, GETREF)

	PRI_M(u32, BrushSize, GET)
	PRI_M(u32, MaxBrushSize, NONE)

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