#include "Command/DefaultCommands.hpp"
#include "Command/CommandLine.hpp"

void DefaultCommands::registerDefaultCommands()
{
    auto* cmd = CommandLine::getInstance();

    cmd->registerCommand("echo", [](const Command& command)
    {
        ECHO(command.getArgumentsString());
    });
}