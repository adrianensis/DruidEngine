#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "UI/UIStyle.hpp"

class GameObject;
class UIButton;
class Material;

class EditorController;

CLASS(UIStyleAtlasButton, UIStyle)
{
public:
	UIStyleAtlasButton()
	{
		mColor = Vector4(0,0,0,1);
		mColorSelected = Vector4(0.3f, 0.3f, 0.3f, 1);
		mColorHovered = Vector4(0.7f, 0.7f, 0.7f, 1);
	}
};

CLASS(Atlas, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(AtlasSelectorUIGroup, NONE, std::string);
	PRI(AtlasUIGroup, NONE, std::string);
	PRI(IsVisible, GET, bool)

private:

	void createAtlas(Material* material);

public:

	Atlas();
	virtual ~Atlas() override;

	void init(EditorController* editorController);
	void toggle();
};