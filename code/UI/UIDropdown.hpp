#pragma once

#include "UI/UIButton.hpp"


class UIDropdown;

class UIDropdownButton: public UIButton
{
    GENERATE_METADATA(UIDropdownButton)
	PRI_M(UIDropdown *, ParentDropdown, GET_SET)
PUB
	virtual void onPostReleased() override;
};

class UIDropdown: public UIButton
{
    GENERATE_METADATA(UIDropdown)
friend UIDropdownButton;

PRI
	class UIDropdownEntry: public ObjectBase
	{
		GENERATE_METADATA(UIDropdown::UIDropdownEntry)
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