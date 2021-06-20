#pragma once

#include "UI/UIElement.hpp"

class UIText;

CLASS(UIButton, UIElement)
{
	PRI(Label, NONE, UIText *)

public:
	virtual void init() override;
	virtual void onDestroy() override;

	void onLabelDestroy();

	virtual void setText(const std::string &text) override;
	virtual void setVisibility(bool visibility) override;
};