#pragma once

#include "UI/UIButton.hpp"

namespace DE {

class UIDropdown : public UIButton {

private:

	class UIDropdownEntry : public ObjectBase{
	public:

		GENERATE_METADATA(UIDropdownEntry);

		UIDropdownEntry();
		virtual ~UIDropdownEntry() override;

		PUBLIC(Label, NONE, String)
		PUBLIC(Callback, NONE, UIElementCallback)

		UIDropdownEntry(String label, UIElementCallback callback);

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

	void setEntriesVisibility(bool visible);

public:

	GENERATE_METADATA(UIDropdown);

	UIDropdown();
	virtual ~UIDropdown() override;

	virtual void init() override;
	virtual void onDestroy() override;

	UIDropdown* addOption(StringRef label, UIElementCallback onPressedCallback);

	virtual void toggle() override;
};
}

