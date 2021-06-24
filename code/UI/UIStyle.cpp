#include "UI/UIStyle.hpp"

void UIStyleManager::init()
{
    mDefaultStyle.mColor = Vector4(0.5f, 0.5f, 0.5f, 1);
	mDefaultStyle.mColorSelected = Vector4(0.3f, 0.3f, 0.3f, 1);
	mDefaultStyle.mColorHovered = Vector4(0.7f, 0.7f, 0.7f, 1);
}