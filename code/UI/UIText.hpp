#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

class UIText : public UIElement {

	u32 mLayer;
	Vector2 mSize;
	String mString;

public:

	GENERATE_METADATA(UIText);

	UIText();
	virtual ~UIText() override;

	virtual void init() override;
	virtual void onDestroy() override;

	virtual void setText(const String &text) override;

	void setLayer(u32 layer) {
		mLayer = layer;
	};

	void setSize(const Vector2 &size) {
		mSize = size;
	};

	StringRef getText() const {
		return mString;
	};
};

}

