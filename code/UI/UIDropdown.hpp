#ifndef DE_UIDROPDOWN_H
#define DE_UIDROPDOWN_H

#include "UI/UIButton.hpp"

namespace DE {

class UIDropdown: public UIButton {

private:

	List<UIButton*>* mButtons;

public:

	DE_CLASS(UIDropdown)

	virtual void init() override;

	void addOption(const std::string& label);

	virtual void toggle() override;
};

} /* namespace DE */

#endif /* DE_UIDROPDOWN_H */
