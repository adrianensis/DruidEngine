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
	PUB const UIStyle* mStyle = nullptr;
	PUB ClassId mUIElementClassId;
	PUB Vector2 mPosition;
	PUB Vector2 mDisplayPosition;
	PUB Vector2 mSize;
	PUB Vector2 mTextSize;
	PUB std::string mText;
	PUB bool mAdjustSizeToText = false;
	PUB i32 mLayer = 0;
	PUB bool mIsAffectedByLayout = false;
	PUB Material* mMaterial = nullptr;
	PUB std::string mGroup;
	PUB GameObject* mParent = nullptr;
	PUB f32 mSeparatorSize = 0.0f;

PUB
	UIElementConfig();

	void init(const Vector2 &position, const Vector2 &size, i32 layer, std::string text = std::string());

	COPY(UIElementConfig)
	{
		DO_COPY(mStyle)
		DO_COPY(mUIElementClassId)
		DO_COPY(mPosition)
		DO_COPY(mDisplayPosition)
		DO_COPY(mSize)
		DO_COPY(mTextSize)
		DO_COPY(mAdjustSizeToText)
		DO_COPY(mLayer)
		DO_COPY(mIsAffectedByLayout)
		DO_COPY(mMaterial)
		DO_COPY(mSeparatorSize)
		DO_COPY(mParent)

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