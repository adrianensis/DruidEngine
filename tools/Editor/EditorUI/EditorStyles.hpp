#pragma once

#include "UI/UIStyle.hpp"

class UIStyleEditor: public UIStyle
{
    GENERATE_METADATA(UIStyleEditor)
PUB
	UIStyleEditor()
	{
		
	}
};

class UIStyleEditorBrushPreview: public UIStyleEditor
{
    GENERATE_METADATA(UIStyleEditorBrushPreview)
PUB
	UIStyleEditorBrushPreview()
	{
		mBackgroundColor = Vector4(0, 0, 0, 0.45f);
	}
};

class UIStyleEditorToolsBar: public UIStyleEditor
{
    GENERATE_METADATA(UIStyleEditorToolsBar)
PUB
	UIStyleEditorToolsBar()
	{
		mBackgroundColor = Vector4(0, 0, 0, 1);
		mColorPressed = Vector4(-0.2f, -0.2f, -0.2f, 1);
		mColorHovered = Vector4(0.4f, 0.4f, 0.4f, 1);
	}
};