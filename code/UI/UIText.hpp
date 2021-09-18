#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"

CLASS(UIText, UIElement)
{
	PRI(Layer, NONE, u32)
	PRI(Size, NONE, Vector2)
	PRI(String, NONE, SStr)
	PRI(FontRenderers, NONE, SVec<Renderer *>)

public:
	VIR void init() OVR;
	VIR void initFromConfig(CNS UIElementConfig& config) OVR;
	VIR void onDestroy() OVR;

	VIR void setText(CNS SStr &text) OVR;

	void setLayer(u32 layer)
	{
		mLayer = layer;
	};

	void setSize(CNS Vector2 &size)
	{
		mSize = size;
	};

	CNS SStr &getText() CNS
	{
		return mString;
	};
};