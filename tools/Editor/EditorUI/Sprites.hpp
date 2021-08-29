#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"

class GameObject;
class UIButton;
class Material;

class EditorController;

CLASS(Sprites, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(SpritesSelectorUIGroup, NONE, std::string)
	PRI(SpritesUIGroup, NONE, std::string)
	PRI(IsVisible, GET, bool)

private:

	void createSprites(Material* material);
	void createSpritesSelectors();

public:

	Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
};