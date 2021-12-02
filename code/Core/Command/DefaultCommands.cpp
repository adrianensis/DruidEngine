#include "Core/Command/DefaultCommands.hpp"
#include "Core/Command/CommandLine.hpp"

#include "Core/Log/Log.hpp"

void DefaultCommands::registerDefaultCommands()
{
    CommandLine& cmd = CommandLine::getInstance();

    cmd.registerCommand("echo", [](const Command& command)
    {
        ECHO(command.getArgumentsString());
    });
}