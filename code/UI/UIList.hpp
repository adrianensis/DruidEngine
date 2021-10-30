#pragma once

#include "UI/UIButton.hpp"


CLASS(UIList, UIElement)
{

PRI
	CLASS_NESTED(UIListEntry, ObjectBase)
	{
		PUB_M(SStr, Label, NONE)
		PUB_M(UIElementCallback, Callback, NONE)

	PUB
		UIListEntry(const SStr& label, UIElementCallback callback);

		COPY(UIListEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	SLst(UIButton *) mButtons;
	SLst(UIListEntry) mEntries;

	void setEntriesVisibility(bool visible);

PUB
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	UIList& addOption(const SStr & label, UIElementCallback onPressedCallback);

	void toggle();
	virtual void onScroll(f32 scroll) override;
};