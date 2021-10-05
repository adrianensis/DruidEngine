#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Command/Command.hpp"
#include "Core/Functor.hpp"


class UIText;

using CommandCallback = SFun(void(CNS Command& command));

CLASS(CommandFunctor, Functor<CommandCallback>)
{
    PRI(Command, Command, GETREF_SET);

public:

	void execute() OVR
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
    PRI(SMap(SStr, CommandFunctor), CommandsMap, NONE)
	PRI(SStr, Buffer, GETREF_CONST)

	PRI(SLst(SStr), History, NONE)
	PRI(SLst(SStr)::iterator, HistoryIterator, NONE)

	PRI(UIText*, UIText, NONE);

public:
    void init();
	void update();
    void terminate();

    void execute(CNS SStr &commandLine);
    SStr autocomplete(CNS SStr &commandLine);
    void registerCommand(CNS SStr &commandName, CommandCallback callback);

	// UI
	void open();
	void close();

};