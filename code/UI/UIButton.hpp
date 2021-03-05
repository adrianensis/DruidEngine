#pragma once

#include "UI/UIElement.hpp"

namespace DE {

class UIText;

class UIButton: public UIElement {

private:

	UIText* mLabel;

public:

	DE_CLASS_BODY(UIButton)

	virtual void init() override;
	virtual void onDestroy() override;

	void onLabelDestroy();

	virtual void setText(const String &text) override;
	virtual void setVisibility(bool visibility) override;

};

}

