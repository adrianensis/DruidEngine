#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"

CLASS(UIText, UIElement)
{
	PRI(u32, Layer, NONE)
	PRI(Vector2, Size, NONE)
	PRI(SStr, String, NONE)
	PRI(SVec(Renderer *), FontRenderers, NONE)

public:
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
};