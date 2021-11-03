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

class UIElementConfig: public ObjectBase
{
    GENERATE_METADATA(UIElementConfig)
	PUB_M(const UIStyle*, Style, NONE)
	PUB_M(ClassId, UIElementClassId, NONE)
	PUB_M(Vector2, Position, NONE)
	PUB_M(Vector2, DisplayPosition, NONE)
	PUB_M(Vector2, Size, NONE)
	PUB_M(Vector2, TextSize, NONE)
	PUB_M(SStr, Text, NONE)
	PUB_M(bool, AdjustSizeToText, NONE)
	PUB_M(u32, Layer, NONE)
	PUB_M(bool, IsAffectedByLayout, NONE)
	PUB_M(Material *, Material, NONE)
	PUB_M(SStr, Group, NONE)
	PUB_M(GameObject*, Parent, NONE)
	PUB_M(f32, SeparatorSize, NONE)

PUB
	UIElementConfig();

	void init(const Vector2 &position, const Vector2 &size, u32 layer, SStr text = SStr());

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