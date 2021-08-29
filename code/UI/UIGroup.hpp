#pragma once

#include "Core/Core.hpp"


class UIElement;

CLASS(UIGroup, ObjectBase)
{
	PUB(Name, GETREF_CONST, std::string)
	PUB(Visible, GET, bool)
	PUB(UIElements, GETREF_CONST, std::list<UIElement *>);

public:
	void init();
};