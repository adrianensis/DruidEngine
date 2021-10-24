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
    virtual void setBackground(const UIElementConfig& config) { };

public:
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