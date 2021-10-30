#pragma once

#include "UI/UIButton.hpp"


class UIDropdown;

CLASS(UIDropdownButton, UIButton)
{
	PRI_M(UIDropdown *, ParentDropdown, GET_SET)
PUB
	virtual void onPostReleased() override;
};

CLASS(UIDropdown, UIButton)
{
friend UIDropdownButton;

PRI
	CLASS_NESTED(UIDropdownEntry, ObjectBase)
	{
		PUB_M(SStr, Label, NONE)
		PUB_M(UIElementCallback, Callback, NONE)

	PUB
		UIDropdownEntry(const SStr& label, UIElementCallback callback);

		COPY(UIDropdownEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	SLst(UIDropdownButton *) mButtons;
	SLst(UIDropdownEntry) mEntries;

	void setEntriesVisibility(bool visible);

PUB
	virtual void init() override;
	virtual void onDestroy() override;

	virtual void onPostReleased() override;

	UIDropdown& addOption(const SStr & label, UIElementCallback onPressedCallback);
	void toggle();
};