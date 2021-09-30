#pragma once

#include "UI/UIElement.hpp"

class UIText;

CLASS(UIButton, UIElement)
{
	PRI(UIText *, Text, GET)

public:
	VIR void init() OVR;
	VIR void initFromConfig(CNS UIElementConfig& config) OVR;
	VIR void onDestroy() OVR;

	void onLabelDestroy();

	VIR void setText(CNS SStr &text) OVR;
	VIR void setVisibility(bool visibility) OVR;
};

CLASS(UIToggleButton, UIButton)
{
public:
	VIR void init() OVR;
};