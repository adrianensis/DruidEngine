#pragma once

#include "UI/UIButton.hpp"


class UIDropdown;

CLASS(UIDropdownButton, UIButton)
{
	PRI(ParentDropdown, GET_SET, UIDropdown *)
public:
	VIR void onReleased() OVR;
};

CLASS(UIDropdown, UIButton)
{
friend UIDropdownButton;

private:
	CLASS_NESTED(UIDropdownEntry, ObjectBase)
	{
		PUB(Label, NONE, SStr)
		PUB(Callback, NONE, UIElementCallback)

	public:
		UIDropdownEntry(CNS SStr& label, UIElementCallback callback);

		COPY(UIDropdownEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	SLst<UIDropdownButton *> mButtons;
	SLst<UIDropdownEntry> mEntries;

	void setEntriesVisibility(bool visible);

public:
	VIR void init() OVR;
	VIR void onDestroy() OVR;

	VIR void onReleased() OVR;

	UIDropdown& addOption(CNS SStr & label, UIElementCallback onPressedCallback);
	void toggle();
};