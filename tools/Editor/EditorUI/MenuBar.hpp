#pragma once

#include "Core/Core.hpp"

#include "EditorUIElement.hpp"

CLASS(MenuBar, EditorUIElement)
{
PUB
	void init(EditorController* editorController) override;
};