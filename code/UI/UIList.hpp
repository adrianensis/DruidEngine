#pragma once

#include "UI/UIButton.hpp"


CLASS(UIList, UIElement)
{

private:
	CLASS_NESTED(UIListEntry, ObjectBase)
	{
		PUB(Label, NONE, SStr)
		PUB(Callback, NONE, UIElementCallback)

	public:
		UIListEntry(CNS SStr& label, UIElementCallback callback);

		COPY(UIListEntry)
		{
			DO_COPY(Label)
			DO_COPY(Callback)
		}
	};

	SLst<UIButton *> mButtons;
	SLst<UIListEntry> mEntries;

	void setEntriesVisibility(bool visible);

public:
	VIR void init() OVR;
	VIR void initFromConfig(CNS UIElementConfig& config) OVR;
	VIR void onDestroy() OVR;

	UIList& addOption(CNS SStr & label, UIElementCallback onPressedCallback);

	void toggle();
	VIR void onScroll(f32 scroll) OVR;
};