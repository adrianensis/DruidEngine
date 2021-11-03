#pragma once

#include "Core/Core.hpp"

class EditorController;

CLASS(EditorUIElement, ObjectBase)
{
	PRI_M(EditorController*, EditorController, GET);
	PRI_M(bool, IsVisible, GET)

PUB
	EditorUIElement()
	{
		mIsVisible = true;
	}

	virtual void init(EditorController* editorController) { mEditorController = editorController; };
    
	virtual void toggle()
	{ 
		setVisibility(!mIsVisible);
	}

	virtual void setVisibility(bool visible) { mIsVisible = visible; };
};