#pragma once

#include "UI/UIStyle.hpp"

CLASS(UIStyleEditor, UIStyle)
{
PUB
	UIStyleEditor()
	{
		
	}
};

CLASS(UIStyleEditorBrushPreview, UIStyleEditor)
{
PUB
	UIStyleEditorBrushPreview()
	{
		mBackgroundColor = Vector4(0, 0, 0, 0.45f);
	}
};

CLASS(UIStyleEditorToolsBar, UIStyleEditor)
{
PUB
	UIStyleEditorToolsBar()
	{
		mBackgroundColor = Vector4(0, 0, 0, 1);
		mColorPressed = Vector4(-0.2f, -0.2f, -0.2f, 1);
		mColorHovered = Vector4(0.4f, 0.4f, 0.4f, 1);
	}
};