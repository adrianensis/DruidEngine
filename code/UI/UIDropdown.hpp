#pragma once

#include "UI/UIButton.hpp"

#include <list>

CLASS(UIDropdown, UIButton)
{

private:
	CLASS_NESTED(UIDropdownEntry, ObjectBase)
	{
		PUB(Label, NONE, std::string)
		PUB(Callback, NONE, UIElementCallback)

	public:
		UIDropdownEntry(const std::string& label, UIElementCallback callback);

		COPY(UIDropdownEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	std::list<UIButton *> mButtons;
	std::list<UIDropdownEntry> mEntries;

	void setEntriesVisibility(bool visible);

public:
	virtual void init() override;
	virtual void onDestroy() override;

	UIDropdown& addOption(const std::string & label, UIElementCallback onPressedCallback);

	virtual void toggle() override;
};