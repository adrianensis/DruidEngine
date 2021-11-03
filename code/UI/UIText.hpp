#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"

class UIPanel;

class UIText: public UIElement
{
    GENERATE_METADATA(UIText)
	PRI u32 mLayer = {};
	PRI Vector2 mSize = {};
	PRI std::string mString = {};
	PRI std::vector<Renderer *> mFontRenderers = {};
    PRI bool mIsEditable = {}; GET(IsEditable)
    PRO UIPanel* mBackground = {};

PRO
    void setIsEditable(bool editable);
    virtual void setBackground(const UIElementConfig& config) { };

PUB
    UIText();

	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	virtual void setText(const std::string &text) override;

	void setLayer(u32 layer)
	{
		mLayer = layer;
	};

	void setSize(const Vector2 &size)
	{
		mSize = size;
	};

	const std::string &getText() const
	{
		return mString;
	};

    virtual void setVisibility(bool visibility) override;
};