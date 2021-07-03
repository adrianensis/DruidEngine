#include "UI/UIElementConfig.hpp"
#include "UI/UI.hpp"

UIElementConfig::UIElementConfig()
{
	mIsAffectedByLayout = true;
}

void UIElementConfig::init(const Vector2 &position, const Vector2 &size, u32 layer, std::string text /*= std::string()*/)
{
	mElementType = UIElementType::PANEL;
	mPosition = position;
	mDisplayPosition = Vector2(0, 0);
	mSize = size;
	mText = text;
	mLayer = layer;
	mTextSize = UI::getDefaultFontSize(); // TODO : move to config?
	mAdjustSizeToText = false;
	mIsAffectedByLayout = true;
	mGroup = "";
	mParent = nullptr;
	mSeparatorSize = 0.01f;
}