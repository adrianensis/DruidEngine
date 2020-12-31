#ifndef DE_UITEXT_H
#define DE_UITEXT_H

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

class UIText: public UIElement {

	u32 mLayer;
	Vector2 mSize;
	std::string mString;

public:

	DE_CLASS(UIText, UIElement)

	virtual void init();

	virtual void setText(const std::string &text) override;

	void setLayer(u32 layer) {
		mLayer = layer;
	};

	void setSize(const Vector2 &size) {
		mSize = size;
	};

	const std::string& getText() const {
		return mString;
	};


};

} /* namespace DE */

#endif /* DE_UITEXT_H */
