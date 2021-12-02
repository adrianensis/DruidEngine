#pragma once

#include "Core/Core.hpp"
#include "EditorUIElement.hpp"

class MenuBar: public EditorUIElement
{
    GENERATE_METADATA(MenuBar)
PUB
	void init(EditorController* editorController) override;
};