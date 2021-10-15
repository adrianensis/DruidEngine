#pragma once

#include "UI/UIStyle.hpp"

CLASS(UIStyleEditor, UIStyle)
{
public:
	UIStyleEditor()
	{
		
	}
};

CLASS(UIStyleEditorBrushPreview, UIStyleEditor)
{
public:
	UIStyleEditorBrushPreview()
	{
		mBackgroundColor = Vector4(0, 0, 0, 0.45f);
	}
};

CLASS(UIStyleEditorToolsBar, UIStyleEditor)
{
public:
	UIStyleEditorToolsBar()
	{
		mBackgroundColor = Vector4(0, 0, 0, 1);
		mColorPressed = Vector4(-0.2f, -0.2f, -0.2f, 1);
		mColorHovered = Vector4(0.4f, 0.4f, 0.4f, 1);
	}
};