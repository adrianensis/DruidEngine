#ifndef DE_UIBUTTON_H
#define DE_UIBUTTON_H

#include "UI/UIElement.hpp"

namespace DE {

class UIText;

class UIButton: public UIElement {

private:

	UIText* mLabel;

public:

	DE_CLASS(UIButton)

	virtual void init();

	virtual void setText(const std::string &text) override;
	virtual void setVisibility(bool visibility) override;

};

} /* namespace DE */

#endif /* DE_UIBUTTON_H */
