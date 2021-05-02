#pragma once

#include "UI/UIText.hpp"

namespace DE {

class UITextEditable : public UIText {

public:

	GENERATE_METADATA(UITextEditable);

	UITextEditable();
	virtual ~UITextEditable() override;

	virtual void init();
	virtual void onFocusLost();

};
}

