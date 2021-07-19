#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "UI/UIStyle.hpp"

class GameObject;
class UIButton;
class Material;

class MapEditor;

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
	PRI(AtlasSelectorUIGroup, NONE, std::string);
	PRI(AtlasUIGroup, NONE, std::string);
	PRI(IsAtlasShow, NONE, bool)

private:

	void createAtlas(Material* material);

public:

	Atlas();
	virtual ~Atlas() override;

	void init();
	void toggle();
};