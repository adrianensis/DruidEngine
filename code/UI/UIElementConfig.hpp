#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"


class Material;
class Scene;
class GameObject;
class UIStyle;

enum class UILayout
{
	HORIZONTAL,
	VERTICAL
};

CLASS(UIElementConfig, ObjectBase)
{
	PUB(CNS UIStyle*, Style, NONE)
	PUB(ClassId, UIElementClassId, NONE)
	PUB(Vector2, Position, NONE)
	PUB(Vector2, DisplayPosition, NONE)
	PUB(Vector2, Size, NONE)
	PUB(Vector2, TextSize, NONE)
	PUB(SStr, Text, NONE)
	PUB(bool, AdjustSizeToText, NONE)
	PUB(u32, Layer, NONE)
	PUB(bool, IsAffectedByLayout, NONE)
	PUB(Material *, Material, NONE)
	PUB(SStr, Group, NONE)
	PUB(GameObject*, Parent, NONE)
	PUB(f32, SeparatorSize, NONE)

public:
	UIElementConfig();

	void init(CNS Vector2 &position, CNS Vector2 &size, u32 layer, SStr text = SStr());

	COPY(UIElementConfig)
	{
		DO_COPY(Style)
		DO_COPY(UIElementClassId)
		DO_COPY(Position)
		DO_COPY(DisplayPosition)
		DO_COPY(Size)
		DO_COPY(TextSize)
		DO_COPY(AdjustSizeToText)
		DO_COPY(Layer)
		DO_COPY(IsAffectedByLayout)
		DO_COPY(Material)
		DO_COPY(SeparatorSize)
		DO_COPY(Parent)

		if (!other->mText.empty())
		{
			mText = other->mText;
		}
		if (!other->mGroup.empty())
		{
			mGroup = other->mGroup;
		}
	}
};