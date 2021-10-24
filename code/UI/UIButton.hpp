#pragma once

#include "UI/UIElement.hpp"

class UIText;

CLASS(UIButton, UIElement)
{
	PRI(UIText *, Text, GET)

public:
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	void onLabelDestroy();

	virtual void setText(const SStr &text) override;
	virtual void setVisibility(bool visibility) override;
};

CLASS(UIToggleButton, UIButton)
{
public:
	virtual void init() override;
};