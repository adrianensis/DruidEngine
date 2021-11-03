#pragma once

#include "Core/Core.hpp"

class UIElement;

class UIGroup: public ObjectBase
{
    GENERATE_METADATA(UIGroup)
	PUB std::string mName = {}; GETREF_CONST(Name)
	PUB bool mVisible = {}; GET(Visible)
	PUB std::list<UIElement *> mUIElements = {}; GETREF_CONST(UIElements);

PUB
	void init();

	void addUIElement(UIElement * uiElement);
	void removeUIElement(UIElement * uiElement);
    void destroyUIElement(UIElement * uiElement);
	void destroyAllUIElements();
	void setVisibility(bool visibility);
};