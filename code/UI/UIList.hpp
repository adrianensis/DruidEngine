#pragma once

#include "UI/UIElement.hpp"

namespace DE {

class UIList: public UIElement {

public:

	DE_CLASS_BODY(UIList)

	virtual void init();

	virtual void setText(const String &text) override;
};

}

