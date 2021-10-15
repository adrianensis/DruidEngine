#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"

class UIPanel;

CLASS(UIText, UIElement)
{
	PRI(u32, Layer, NONE)
	PRI(Vector2, Size, NONE)
	PRI(SStr, String, NONE)
	PRI(SVec(Renderer *), FontRenderers, NONE)
    PRI(bool, IsEditable, GET)
    PRO(UIPanel*, Background, NONE)

protected:
    void setIsEditable(bool editable);
    VIR void setBackground(const UIElementConfig& config) { };

public:
    UIText();

	VIR void init() OVR;
	VIR void initFromConfig(const UIElementConfig& config) OVR;
	VIR void onDestroy() OVR;

	VIR void setText(const SStr &text) OVR;

	void setLayer(u32 layer)
	{
		mLayer = layer;
	};

	void setSize(const Vector2 &size)
	{
		mSize = size;
	};

	const SStr &getText() const
	{
		return mString;
	};

    VIR void setVisibility(bool visibility) OVR;
};