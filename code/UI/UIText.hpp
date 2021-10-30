#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"

class UIPanel;

CLASS(UIText, UIElement)
{
	PRI_M(u32, Layer, NONE)
	PRI_M(Vector2, Size, NONE)
	PRI_M(SStr, String, NONE)
	PRI_M(SVec(Renderer *), FontRenderers, NONE)
    PRI_M(bool, IsEditable, GET)
    PRO_M(UIPanel*, Background, NONE)

PRO
    void setIsEditable(bool editable);
    virtual void setBackground(const UIElementConfig& config) { };

PUB
    UIText();

	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	virtual void setText(const SStr &text) override;

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

    virtual void setVisibility(bool visibility) override;
};