#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"
#include "EditorUIElement.hpp"

class ToolsBar: public EditorUIElement
{
    GENERATE_METADATA(ToolsBar)
PUB
	void init(EditorController* editorController) override;
	void setVisibility(bool visible) override;
};