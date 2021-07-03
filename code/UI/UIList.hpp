#pragma once

#include "UI/UIButton.hpp"

#include <list>

CLASS(UIList, UIElement)
{

private:
	CLASS(UIListEntry, ObjectBase)
	{
		PUB(Label, NONE, std::string)
		PUB(Callback, NONE, UIElementCallback)

	public:
		UIListEntry(const std::string& label, UIElementCallback callback);

		COPY(UIListEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	std::list<UIButton *> mButtons;
	std::list<UIListEntry> mEntries;

	void setEntriesVisibility(bool visible);

public:
	virtual void init() override;
	virtual void onDestroy() override;

	UIList& addOption(const std::string & label, UIElementCallback onPressedCallback);

	virtual void toggle() override;
	virtual void onScroll(f32 scroll) override;
};