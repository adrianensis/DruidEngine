#pragma once

#include "UI/UIText.hpp"

#include "UI/UIStyle.hpp"

class UIStyleEditableTextBackground: public UIStyle
{
    GENERATE_METADATA(UIStyleEditableTextBackground)
PUB
	UIStyleEditableTextBackground()
	{
		mBackgroundColor = Vector4(0, 0, 0.2f, 1);
	}
};

class UIEditableText: public UIText
{
    GENERATE_METADATA(UIEditableText)
PRO
    virtual void setBackground(const UIElementConfig& config) override;

PUB
    virtual void init() override;
};