#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Command/Command.hpp"
#include "Core/Functor.hpp"


class UIText;

using CommandCallback = SFun(void(const Command& command));

CLASS(CommandFunctor, Functor<CommandCallback>)
{
    PRI_M(Command, Command, GETREF_SET);

PUB

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
    PRI_M(SMap(SStr, CommandFunctor), CommandsMap, NONE)
	PRI_M(SStr, Buffer, GETREF_CONST)

	PRI_M(SLst(SStr), History, NONE)
	PRI_M(SLst(SStr)::iterator, HistoryIterator, NONE)

	PRI_M(UIText*, UIText, NONE);

    PRI_M(bool, IsOpen, NONE);

    void log(const SStr& line, bool newLine = true) const;

PUB
    void init();
	void update();
    void terminate();

    void execute(const SStr &commandLine);
    SStr autocomplete(const SStr &commandLine);
    void registerCommand(const SStr &commandName, CommandCallback callback);

	// UI
	void open();
	void close();
    void toggle();
};