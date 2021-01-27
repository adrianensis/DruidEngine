#pragma once

#include "UI/UIText.hpp"

namespace DE {

class UITextEditable: public UIText {

public:

	DE_CLASS(UITextEditable)

	virtual void init();
	virtual void onFocusLost();

};

}

