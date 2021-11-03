#pragma once

#include "Core/Core.hpp"

#define DECLARE_EDITOR_UIGROUP(GroupName)\
	PUB inline static std::string sm##GroupName = #GroupName;

class EditorUIGroups: public ObjectBase
{
    GENERATE_METADATA(EditorUIGroups)
	DECLARE_EDITOR_UIGROUP(ToolsBar)
	
	DECLARE_EDITOR_UIGROUP(SpritesSelector)
    DECLARE_EDITOR_UIGROUP(Sprites)
    DECLARE_EDITOR_UIGROUP(SpritePreview)
	DECLARE_EDITOR_UIGROUP(Atlas)
    DECLARE_EDITOR_UIGROUP(Frames)

	DECLARE_EDITOR_UIGROUP(LayersBar)
};