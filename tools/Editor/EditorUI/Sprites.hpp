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
	PRI(EditorController*, EditorController, NONE)
	PRI(SStr, SpritesSelectorUIGroup, NONE)
	PRI(SStr, SpritesUIGroup, NONE)
	PRI(bool, IsVisible, GET)
    PRI(Grid, Grid, NONE) // TODO: use this grid to create sprites

private:

	void createSprites(Material* material);
	void createSpritesSelectors();

public:

	Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
};