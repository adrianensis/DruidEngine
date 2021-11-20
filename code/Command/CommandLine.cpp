#include "Command/CommandLine.hpp"

#include "Command/DefaultCommands.hpp"

#include "Input/Input.hpp"
#include "Log/Log.hpp"
#include "Events/EventsManager.hpp"
#include "UI/UI.hpp"
#include "UI/UIText.hpp"
#include <regex>

void CommandLine::init()
{
    mBuffer = "";
    mIsOpen = false;

    SUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this, [this](const Event *event)
	{
        if(mIsOpen)
        {
            const InputEventChar *e = (InputEventChar*) event;
            char c = e->mChar;
            mBuffer.push_back(c);
            log(mBuffer, false);
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this, [this](const Event *event)
	{
        if(mIsOpen)
        {
            BRLINE();
            execute(mBuffer);
            mBuffer.clear();
            log("", false);
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyBackspace, nullptr, this, [this](const Event *event)
	{
        if(mIsOpen)
        {
            if(!mBuffer.empty())
            {
                BACKSPACE()
                mBuffer.pop_back();
            }
            
            log(mBuffer, false);
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyArrow, nullptr, this, [this](const Event *event)
	{
        if(mIsOpen)
        {
            const InputEventKeyArrow *e = (InputEventKeyArrow*) event;

            switch (e->mArrowButton)
            {
                case GLFW_KEY_UP:
                {
                    if(!mHistory.empty() && mHistoryIterator != mHistory.begin())
                    {
                        --mHistoryIterator;
                        mBuffer = *mHistoryIterator;
                    }
                    break;
                }
                case GLFW_KEY_DOWN:
                {
                    if(!mHistory.empty() && mHistoryIterator != mHistory.end())
                    {
                        ++mHistoryIterator;
                        if(mHistoryIterator == mHistory.end())
                        {
                            mBuffer.clear();
                        }
                        else
                        {
                            mBuffer = *mHistoryIterator;
                        }
                    }
                    break;
                }
            }
        }
	});

    DefaultCommands::registerDefaultCommands();
}

void CommandLine::log(const std::string& line, bool newLine /*= true*/) const
{
    if(newLine)
    {
        CUSTOM_ECHO("CMD", line)
    }
    else
    {
        CUSTOM_ECHO_APPEND("CMD", line)
    }
}

void CommandLine::update()
{
    if(mIsOpen)
    {
        //mUIText->setText("> " + mBuffer);
    }
}

void CommandLine::terminate()
{

}

void CommandLine::execute(const std::string &commandLine)
{
    std::string patternValidName("[-+]?[a-zA-Z_\\.0-9]+");
    std::regex regexCommand("^\\s*(" + patternValidName + ")\\s*");

    std::smatch matchCommand;
    std::regex_search(commandLine, matchCommand, regexCommand);
    bool isCommand = !matchCommand.empty();

    std::string commandName = matchCommand[1];

    if(isCommand && MAP_CONTAINS(mCommandsMap, commandName))
    {
        //log("command: " + commandName);

        CommandFunctor& functor = mCommandsMap.at(commandName);
        functor.getCommand().clearArguments();

        std::string patternAssignation("\\s*=\\s*");
        std::regex regexCommandWithArgumentList("^\\s*" + patternValidName + "\\s+((" + patternValidName + "(" + patternAssignation + patternValidName + ")?\\s*)+)\\s*");

        std::smatch matchCommandWithArgumentList;
        std::regex_search(commandLine, matchCommandWithArgumentList, regexCommandWithArgumentList);
        bool isCommandWithArgumentList = !matchCommandWithArgumentList.empty();

        if(isCommandWithArgumentList)
        {
            std::string argumentList = matchCommandWithArgumentList[1].str();

            std::regex regexArgument("\\s*(" + patternValidName + "(" + patternAssignation + patternValidName + ")?)\\s*");

            auto argumentlistBegin = std::sregex_iterator(argumentList.begin(), argumentList.end(), regexArgument);
            auto argumentlistEnd = std::sregex_iterator();
        
            //log("arguments");
            //VAL(std::distance(argumentlistBegin, argumentlistEnd))
        
            functor.getCommand().setArgumentsString(argumentList);

            for (std::sregex_iterator i = argumentlistBegin; i != argumentlistEnd; ++i) {
                std::smatch argumentMatch = *i;                                                 
                std::string argumentStr = argumentMatch.str(); 

                std::regex regexPair("(" + patternValidName + ")(" + patternAssignation + "(" + patternValidName + "))?");

                std::smatch matchPair;
                std::regex_search(argumentStr, matchPair, regexPair);
                //bool isPair = !matchPair.empty();

                //VAL(matchPair[1].str())
                //VAL(matchPair[3].str())

                CommandArgument arg;
                arg.setName(matchPair[1].str());
                arg.setValue(matchPair[3].str());
                functor.getCommand().addArgument(arg);
            }   
        }
        
        functor.execute();
    }
    else
    {
        log("command: " + commandLine + " not recognized.");
    }

    mHistory.push_back(commandLine);
    mHistoryIterator = mHistory.end();
}

std::string CommandLine::autocomplete(const std::string &commandLine)
{
    return std::string(); // TODO : implement command autocomplete
}

void CommandLine::registerCommand(const std::string &commandName, CommandCallback callback)
{
    ASSERT_MSG(!MAP_CONTAINS(mCommandsMap, commandName), "Command " + commandName + " already registered!");

    Command command;
    command.setName(commandName);

    CommandFunctor functor;
    functor.setCallback(callback);
    functor.setCommand(command);

    MAP_INSERT(mCommandsMap, commandName, functor)
}

void CommandLine::open()
{
    mIsOpen = true;
    log("CMD Opened");
    log(mBuffer, false);
    /*if(!mUIText)
    {
        mUIText = uiBuilder.
        setLayout(UILayout::VERTICAL).
        setPosition(Vector2(-1.0f, -0.96f)).
        setDepth(0).
        setAdjustSizeToText(false).
        create<UIText>().
        getUIElement<UIText>();
    }

    if(!mUIText->isActive())
    {
        mUIText->setIsActive(true);
    }*/
}

void CommandLine::close()
{
    mIsOpen = false;
    log(mBuffer);
    log("CMD Closed");
    /*if(mUIText && mUIText->isActive())
    {
        mUIText->setIsActive(false);
    }*/
}

void CommandLine::toggle()
{
    if(mIsOpen)
    {
        close();
    }
    else
    {
        open();
    }
}