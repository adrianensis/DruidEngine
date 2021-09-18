#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Command/Command.hpp"
#include "Core/Functor.hpp"


class UIText;

using CommandCallback = SFun<void(CNS Command& command)>;

CLASS(CommandFunctor, Functor<CommandCallback>)
{
    PRI(Command, GETREF_SET, Command);

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
    PRI(CommandsMap, NONE, SMap<SStr, CommandFunctor>)
	PRI(Buffer, GETREF_CONST, SStr)

	PRI(History, NONE, SLst<SStr>)
	PRI(HistoryIterator, NONE, SLst<SStr>::iterator)

	PRI(UIText, NONE, UIText*);

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