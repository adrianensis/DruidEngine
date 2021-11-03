#include "UI/UIBuilder.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Material/Material.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Scene/Transform.hpp"
#include "Scene/ScenesManager.hpp"
#include "Input/Input.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "UI/UI.hpp"
#include "UI/UIStyle.hpp"
#include "Core/ClassManager.hpp"

UIBuilder::UIBuilder()
{
	mCurrentLayout = UILayout::VERTICAL;
	mMakeRelativeToLastConfig = false;
	mCurrentUIElement = nullptr;

	mDefaultConfig.init(Vector2(0, 0), Vector2(0, 0), 0);
	mConfig = mDefaultConfig;
	//mSavedData.init(Vector2(0,0), Vector2(0,0), "", 0);
}

void UIBuilder::registerUIElement(UIElement *uiElement)
{
	mCurrentUIElement = uiElement;

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(mCurrentUIElement);

	if (mConfig.mGroup.length() > 0)
	{
		UI::getInstance()->getOrCreateGroup(mConfig.mGroup).addUIElement(mCurrentUIElement);
	}

	if (mConfig.mIsAffectedByLayout)
	{
		if (mNewRowOrColumn)
		{
			mNewRowOrColumn = false;
			mLayoutFirstUIElementConfig = mConfig;
		}

		mMakeRelativeToLastConfig = true;

		mLastConfig = mConfig;
	}
}

UILayout UIBuilder::getOppositeLayout(UILayout layout)
{
	return (UILayout)(((int)mCurrentLayout + 1) % 2);
}

Vector2 UIBuilder::calculateNextElementOffset(UILayout layout)
{
	Vector2 offset = Vector2(0, 0);

	switch (layout)
	{
		case UILayout::HORIZONTAL:
		{
			offset = Vector2(mLastConfig.mSize.x / RenderContext::getAspectRatio() + mConfig.mSeparatorSize, 0);
			break;
		}
		case UILayout::VERTICAL:
		{
			offset = Vector2(0, -(mLastConfig.mSize.y + mConfig.mSeparatorSize));
			break;
		}
	}

	return offset;
}

void UIBuilder::calculateConfig()
{
	if (mConfig.mAdjustSizeToText)
	{
		mConfig.mSize.x = (mConfig.mTextSize.x * mConfig.mText.length());
		mConfig.mSize.y = mConfig.mTextSize.y;
	}

	if (mConfig.mIsAffectedByLayout && mMakeRelativeToLastConfig)
	{
		Vector2 offset = calculateNextElementOffset(mNewRowOrColumn ? getOppositeLayout(mCurrentLayout) : mCurrentLayout);
		mConfig.mPosition = mLastConfig.mPosition + offset;
	}

	// Offset the UI Element so its Top-Left corner is the origin.
	mConfig.mDisplayPosition = mConfig.mPosition;

	if(mConfig.mUIElementClassId == UIText::getClassIdStatic() || mConfig.mUIElementClassId == UIEditableText::getClassIdStatic())
	{
		mConfig.mDisplayPosition.x += mConfig.mTextSize.x/RenderContext::getAspectRatio();
		mConfig.mDisplayPosition.y -= mConfig.mTextSize.y / 2.0f;
	}
	else
	{
		mConfig.mDisplayPosition.x += (mConfig.mSize.x/RenderContext::getAspectRatio()) / 2.0f;
		mConfig.mDisplayPosition.y -= mConfig.mSize.y / 2.0f;
	}
}

UIBuilder &UIBuilder::nextRow()
{
	mLastConfig = mLayoutFirstUIElementConfig;
	mNewRowOrColumn = true;
	return *this;
}

UIBuilder &UIBuilder::nextColumn()
{
	return nextRow(); // NOTE : exactly the same code.
}

UIBuilder &UIBuilder::saveData()
{
	mConfigStack.push_front(mConfig);
	return *this;
}

UIBuilder &UIBuilder::restoreData()
{
	mConfig = mConfigStack.front();
	mConfigStack.pop_front();
	return *this;
}

UIBuilder &UIBuilder::create(const std::string &className)
{
	UIElement* uiElement = INSTANCE_BY_NAME(className, UIElement);
	mConfig.mUIElementClassId = uiElement->getClassId();

	calculateConfig();
	uiElement->initFromConfig(mConfig);

	registerUIElement(uiElement);

	return *this;
}