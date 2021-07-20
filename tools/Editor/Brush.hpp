#pragma once

#include "Core/ObjectBase.hpp"
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
	PRI(BrushPreview, NONE, GameObject *)
	PRI(Mode, NONE, BrushMode)
	PRI(PaintMode, NONE, BrushPaintMode)

	PRI(PaintData, GETREF, BrushPaintData)

private:

	void createSelector();
	void setSelectorVisibility(bool visible);
	void createBrushPreview();
	void removeBrushPreview();

public:

	void init(EditorController* editorController);
	void onPressed();
	void onMouseMoved();
	void onTileSelectedFromAtlas(GameObject* tile);
};