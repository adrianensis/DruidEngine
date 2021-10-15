#include "EditorCommands.hpp"
#include "EditorController.hpp"
#include "Command/CommandLine.hpp"

void EditorCommands::registerEditorCommands(EditorController* editorController)
{
    auto* cmd = CommandLine::getInstance();

    cmd->registerCommand("sprite", [&](const Command& command)
    {
        if(command.argumentExists("-n"))
        {
            //editorController->getSprites()
        }
    });
}