#pragma once

#include "UI/UIText.hpp"

#include "UI/UIStyle.hpp"

CLASS(UIStyleEditableTextBackground, UIStyle)
{
public:
	UIStyleEditableTextBackground()
	{
		mBackgroundColor = Vector4(0, 0, 0.2f, 1);
	}
};

CLASS(UIEditableText, UIText)
{
protected:
    virtual void setBackground(const UIElementConfig& config) override;

public:
    virtual void init() override;
};