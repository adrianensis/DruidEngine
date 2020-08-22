#ifndef DE_UIBUTTON_H
#define DE_UIBUTTON_H

#include "UIElement.hpp"

namespace DE {

class UIText;

class UIButton: public UIElement {

private:

	UIText* mLabel;

public:

	DE_CLASS(UIButton, UIElement);

	virtual void setText(const std::string &text) override;
	virtual void setVisibility(bool visibility) override;

};

} /* namespace DE */

#endif /* DE_UIBUTTON_H */
