#pragma once

#include "UI/UIButton.hpp"


class UIList: public UIElement
{
    GENERATE_METADATA(UIList)

PRI
	class UIListEntry: public ObjectBase
	{
		GENERATE_METADATA(UIList::UIListEntry)
		PUB std::string mLabel;
		PUB UIElementCallback mCallback;

	PUB
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

PUB
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	UIList& addOption(const std::string & label, UIElementCallback onPressedCallback);

	void toggle();
	virtual void onScroll(f32 scroll) override;
};