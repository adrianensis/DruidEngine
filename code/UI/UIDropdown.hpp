#ifndef DE_UIDROPDOWN_H
#define DE_UIDROPDOWN_H

#include "UIButton.hpp"

namespace DE {

class UIDropdown: public UIButton {

private:

	List<UIButton*>* mButtons;

public:

	DE_CLASS(UIDropdown, UIButton);

	virtual void init() override;

	void addOption(const std::string& label);

	virtual void toggle() override;
};

} /* namespace DE */

#endif /* DE_UIDROPDOWN_H */
