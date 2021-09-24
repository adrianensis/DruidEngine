#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "EditorUI/Grid.hpp"

class GameObject;
class UIButton;
class Material;

class EditorController;

CLASS(Sprites, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(SpritesSelectorUIGroup, NONE, SStr)
	PRI(SpritesUIGroup, NONE, SStr)
	PRI(IsVisible, GET, bool)
    PRI(Grid, NONE, Grid) // TODO: use this grid to create sprites

private:

	void createSprites(Material* material);
	void createSpritesSelectors();

public:

	Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
};