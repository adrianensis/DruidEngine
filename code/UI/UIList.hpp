#pragma once

#include "UI/UIButton.hpp"


CLASS(UIList, UIElement)
{

private:
	CLASS_NESTED(UIListEntry, ObjectBase)
	{
		PUB(SStr, Label, NONE)
		PUB(UIElementCallback, Callback, NONE)

	public:
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

public:
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config) override;
	virtual void onDestroy() override;

	UIList& addOption(const SStr & label, UIElementCallback onPressedCallback);

	void toggle();
	virtual void onScroll(f32 scroll) override;
};