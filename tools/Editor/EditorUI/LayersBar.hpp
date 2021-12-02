#pragma once

#include "Core/Core.hpp"
#include "EditorUIElement.hpp"

class LayersBar: public EditorUIElement
{
    GENERATE_METADATA(LayersBar)
PUB
	void init(EditorController* editorController) override;
	void setVisibility(bool visible) override;
};