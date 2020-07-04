#ifndef DE_UIBUTTON_H
#define DE_UIBUTTON_H

#include "UIElement.hpp"

namespace DE {

class UIText;

class UIButton: public UIElement {

private:

	UIText *mLabel;

public:

	DE_CLASS(UIButton, UIElement)
	;

	void setLabel(const std::string &label);

};

} /* namespace DE */

#endif /* DE_UIBUTTON_H */
