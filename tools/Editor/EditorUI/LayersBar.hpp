#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

#include "EditorUIElement.hpp"

CLASS(LayersBar, EditorUIElement)
{
PUB
	void init(EditorController* editorController) override;
	void setVisibility(bool visible) override;
};