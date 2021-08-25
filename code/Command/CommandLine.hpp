#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Command/Command.hpp"
#include "Core/Functor.hpp"

#include <string>
#include <map>
#include <list>

class UIText;

using CommandCallback = std::function<void(const Command& command)>;

CLASS(CommandFunctor, Functor<CommandCallback>)
{
    PRI(Command, GETREF_SET, Command);

public:

	void execute() override
	{
		if (mCallback)
		{
			mCallback(mCommand);
		}
	}

	// NOTE : Override in children!
	COPY(CommandFunctor)
	{
		Functor<CommandCallback>::copy(other);
        DO_COPY(Command)
	}

};

CLASS(CommandLine, ObjectBase), SINGLETON(CommandLine)
{
    PRI(CommandsMap, NONE, std::map<std::string, CommandFunctor>)
	PRI(Buffer, GETREF_CONST, std::string)

	PRI(History, NONE, std::list<std::string>)
	PRI(HistoryIterator, NONE, std::list<std::string>::iterator)

	PRI(UIText, NONE, UIText*);

public:
    void init();
	void update();
    void terminate();

    void execute(const std::string &commandLine);
    std::string autocomplete(const std::string &commandLine);
    void registerCommand(const std::string &commandName, CommandCallback callback);

	// UI
	void open();
	void close();

};