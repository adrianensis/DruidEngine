#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"

#include <string>
#include <list>

class Material;
class Scene;
class GameObject;

enum class UIElementType
{
	PANEL,
	BUTTON,
	TEXT,
	TEXTEDITABLE,
	DROPDOWN,
	LIST
};

enum class UILayout
{
	HORIZONTAL,
	VERTICAL,
	MAX
};

CLASS(UIElementConfig, ObjectBase)
{
	PUB(ElementType, NONE, UIElementType)
	PUB(Position, NONE, Vector2)
	PUB(DisplayPosition, NONE, Vector2)
	PUB(Size, NONE, Vector2)
	PUB(TextSize, NONE, Vector2)
	PUB(Text, NONE, std::string)
	PUB(AdjustSizeToText, NONE, bool)
	PUB(Layer, NONE, u32)
	PUB(IsAffectedByLayout, NONE, bool)
	PUB(Group, NONE, std::string)
	PUB(Parent, NONE, GameObject*)
	PUB(SeparatorSize, NONE, f32)

public:
	UIElementConfig();

	void init(const Vector2 &position, const Vector2 &size, u32 layer, std::string text = std::string());

	COPY(UIElementConfig)
	{
		DO_COPY(ElementType)
		DO_COPY(Position)
		DO_COPY(DisplayPosition)
		DO_COPY(Size)
		DO_COPY(TextSize)
		DO_COPY(AdjustSizeToText)
		DO_COPY(Layer)
		DO_COPY(IsAffectedByLayout)
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