#pragma once

#include "Core/Core.hpp"
#include "Command/Command.hpp"

class UIText;

using CommandCallback = std::function<void(const Command& command)>;

class CommandFunctor: public Functor<CommandCallback>
{
	GENERATE_METADATA(CommandFunctor)
	
    PRI Command mCommand; GETREF_SET(Command);

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
        DO_COPY(mCommand)
	}

};

class CommandLine: public ObjectBase, public Singleton<CommandLine>
{
	GENERATE_METADATA(CommandLine)
    PRI std::map<std::string, CommandFunctor> mCommandsMap;
	PRI std::string mBuffer; GETREF_CONST(Buffer)

	PRI std::list<std::string> mHistory;
	PRI std::list<std::string>::iterator mHistoryIterator;

	PRI UIText* mUIText = nullptr;

    PRI bool mIsOpen = false;

    void log(const std::string& line, bool newLine = true) const;

PUB
    void init();
	void update();
    void terminate();

    void execute(const std::string &commandLine);
    std::string autocomplete(const std::string &commandLine);
    void registerCommand(const std::string &commandName, CommandCallback callback);

	// UI
	void open();
	void close();
    void toggle();
};