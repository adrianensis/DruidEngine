#include "UI/UIStyle.hpp"

UIStyleManager::~UIStyleManager()
{
	MAP_DELETE_CONTENT(mStyles)
}

void UIStyleManager::init()
{
    mDefaultStyle.mTextColor = Vector4(0.0f, 0.0f, 0.0f, 1);
    mDefaultStyle.mBackgroundColor = Vector4(0.5f, 0.5f, 0.5f, 1);
	mDefaultStyle.mColorPressed = Vector4(0.3f, 0.3f, 0.3f, 1);
	mDefaultStyle.mColorHovered = Vector4(0.7f, 0.7f, 0.7f, 1);
}