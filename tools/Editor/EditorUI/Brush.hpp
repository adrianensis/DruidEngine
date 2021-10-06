#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"


class GameObject;
class EditorController;
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
	PUB(Rectangle, Region, NONE)
	PUB(Material *, Material, NONE)
};

CLASS(Brush, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
	PRI(GameObject *, Selector, NONE)
	PRI(SVec(GameObject *), BrushPreview, NONE)

	PRI(BrushMode, Mode, NONE)
	PRI(BrushPaintMode, PaintMode, NONE)

	PRI(BrushPaintData, PaintData, GETREF)

	PRI(u32, BrushSize, GET)
	PRI(u32, MaxBrushSize, NONE)

private:

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

public:

	void init(EditorController* editorController);
	void onTileSelectedFromAtlas(GameObject* tile);

	void setModeSelect();
	void setModePaint();
	void setModeErase();
	void setBrushSize(u32 size);
};