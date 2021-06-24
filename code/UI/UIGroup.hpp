#pragma once

#include "Core/ObjectBase.hpp"

#include <string>
#include <list>

class UIElement;

CLASS(UIGroup, ObjectBase)
{
	PUB(Name, NONE, std::string)
	PUB(Visible, NONE, bool)
	PUB(UIElements, NONE, std::list<UIElement *>);

public:
	void init();
};