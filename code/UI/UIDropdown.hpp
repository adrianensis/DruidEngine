#pragma once

#include "UI/UIButton.hpp"


class UIDropdown;

CLASS(UIDropdownButton, UIButton)
{
	PRI(UIDropdown *, ParentDropdown, GET_SET)
public:
	virtual void onPostReleased() override;
};

CLASS(UIDropdown, UIButton)
{
friend UIDropdownButton;

private:
	CLASS_NESTED(UIDropdownEntry, ObjectBase)
	{
		PUB(SStr, Label, NONE)
		PUB(UIElementCallback, Callback, NONE)

	public:
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

public:
	virtual void init() override;
	virtual void onDestroy() override;

	virtual void onPostReleased() override;

	UIDropdown& addOption(const SStr & label, UIElementCallback onPressedCallback);
	void toggle();
};