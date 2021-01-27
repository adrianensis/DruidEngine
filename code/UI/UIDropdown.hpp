#pragma once

#include "UI/UIButton.hpp"

namespace DE {

class UIDropdown: public UIButton {

private:

	class UIDropdownEntry : public DE_Class{
	public:

		DE_CLASS(UIDropdownEntry)

		DE_M(Label, std::string);
		DE_M(Callback, UIElementCallback);

		UIDropdownEntry(std::string label, UIElementCallback callback);

		UIDropdownEntry& operator= (const UIDropdownEntry &other) {
			// self-assignment guard
			if (this == &other)
				return *this;

			// do the copy
			mLabel = other.mLabel;
			mCallback = other.mCallback;

			return *this;
		}
	};

	List<UIButton*>* mButtons;
	List<UIDropdownEntry>* mEntries;

public:

	DE_CLASS(UIDropdown)

	virtual void init() override;
	virtual void onDestroy() override;

	UIDropdown* addOption(const std::string& label, UIElementCallback onPressedCallback);

	virtual void toggle() override;
};

}

