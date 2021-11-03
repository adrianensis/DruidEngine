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
	PUB const UIStyle* mStyle = {};
	PUB ClassId mUIElementClassId = {};
	PUB Vector2 mPosition = {};
	PUB Vector2 mDisplayPosition = {};
	PUB Vector2 mSize = {};
	PUB Vector2 mTextSize = {};
	PUB std::string mText = {};
	PUB bool mAdjustSizeToText = {};
	PUB u32 mLayer = {};
	PUB bool mIsAffectedByLayout = {};
	PUB Material * mMaterial = {};
	PUB std::string mGroup = {};
	PUB GameObject* mParent = {};
	PUB f32 mSeparatorSize = {};

PUB
	UIElementConfig();

	void init(const Vector2 &position, const Vector2 &size, u32 layer, std::string text = std::string());

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