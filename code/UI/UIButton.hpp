#pragma once

#include "UI/UIElement.hpp"

class UIText;

class UIButton: public UIElement
{
    GENERATE_METADATA(UIButton)
	PRI_M(UIText *, Text, GET)

PUB
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	void onLabelDestroy();

	virtual void setText(const SStr &text) override;
	virtual void setVisibility(bool visibility) override;
};

class UIToggleButton: public UIButton
{
    GENERATE_METADATA(UIToggleButton)
PUB
	virtual void init() override;
};