#ifndef DE_UILIST_H
#define DE_UILIST_H

#include "UIElement.hpp"

namespace DE {

class UIList: public UIElement {

public:

	DE_CLASS(UIList, UIElement);

	virtual void setText(const std::string &text) override;
};

} /* namespace DE */

#endif /* DE_UILIST_H */
