#pragma once

#include "UI/UIElement.hpp"

class UIText;

CLASS(UIButton, UIElement)
{
	PRI(Text, GET, UIText *)

public:
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	void onLabelDestroy();

	virtual void setText(const std::string &text) override;
	virtual void setVisibility(bool visibility) override;
};

CLASS(UIToggleButton, UIButton)
{
public:
	virtual void init() override;
};