#pragma once

#include "UI/UIText.hpp"

#include "UI/UIStyle.hpp"

CLASS(UIStyleEditableTextBackground, UIStyle)
{
PUB
	UIStyleEditableTextBackground()
	{
		mBackgroundColor = Vector4(0, 0, 0.2f, 1);
	}
};

CLASS(UIEditableText, UIText)
{
PRO
    virtual void setBackground(const UIElementConfig& config) override;

PUB
    virtual void init() override;
};