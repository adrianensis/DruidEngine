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
	PUB(Region, NONE, Rectangle)
	PUB(Material, NONE, Material *)
};

CLASS(Brush, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(Selector, NONE, GameObject *)
	PRI(BrushPreview, NONE, std::vector<GameObject *>)

	PRI(Mode, NONE, BrushMode)
	PRI(PaintMode, NONE, BrushPaintMode)

	PRI(PaintData, GETREF, BrushPaintData)

	PRI(BrushSize, GET, u32)
	PRI(MaxBrushSize, NONE, u32)

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