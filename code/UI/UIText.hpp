#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Maths.hpp"

class UIPanel;

class UIText: public UIElement
{
    GENERATE_METADATA(UIText)
	PRI i32 mLayer = 0;
	PRI Vector2 mSize;
	PRI std::string mString;
	PRI std::vector<Renderer *> mFontRenderers;
    PRI bool mIsEditable = false; GET(IsEditable)
    PRO UIPanel* mBackground = nullptr;

PRO
    void setIsEditable(bool editable);
    virtual void setBackground(const UIElementConfig& config) { };

PUB
    UIText();

	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	virtual void setText(const std::string &text) override;

	void setLayer(i32 layer)
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