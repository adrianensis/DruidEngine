#pragma once

#include "Core/Core.hpp"

class EditorController;

class EditorCommands
{
PUB
    static void registerEditorCommands(EditorController* editorController);
};