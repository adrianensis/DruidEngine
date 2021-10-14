#include "Command/CommandLine.hpp"

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

    SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [this](const Event *event)
	{
        if(mIsOpen)
        {
            BRLINE();
            mBuffer.clear();
            log("", false);
        }
	});

    /*SUBSCRIBE_TO_EVENT(InputEventKeyDelete, nullptr, this, [this](const Event *event)
	{
        if(!mBuffer.empty())
        {
            mBuffer.pop_back();
        }

        VAR(mBuffer)
	});*/

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

    registerCommand("echo", [](const Command& command)
    {
        FOR_MAP(it, command.getArgs())
        {
            ECHO(it->second.getName() + " = " + it->second.getValue());
        }
    });
}

void CommandLine::log(const SStr& line, bool newLine /*= true*/) const
{
    if(newLine)
    {
        ECHO(line)
    }
    else
    {
        ECHO_APPEND(line)
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

void CommandLine::execute(const SStr &commandLine)
{
    SStr patternValidName("[-+]?[a-zA-Z_\\.0-9]+");
    std::regex regexCommand("^\\s*(" + patternValidName + ")\\s*");

    std::smatch matchCommand;
    std::regex_search(commandLine, matchCommand, regexCommand);
    bool isCommand = !matchCommand.empty();

    SStr commandName = matchCommand[1];

    if(isCommand && MAP_CONTAINS(mCommandsMap, commandName))
    {
        log("command: " + commandName);

        CommandFunctor& functor = mCommandsMap.at(commandName);
        functor.getCommand().clearArgs();

        SStr patternAssignation("\\s*=\\s*");
        std::regex regexCommandWithArgumentList("^\\s*" + patternValidName + "\\s+((" + patternValidName + "(" + patternAssignation + patternValidName + ")?\\s*)+)\\s*");

        std::smatch matchCommandWithArgumentList;
        std::regex_search(commandLine, matchCommandWithArgumentList, regexCommandWithArgumentList);
        bool isCommandWithArgumentList = !matchCommandWithArgumentList.empty();

        if(isCommandWithArgumentList)
        {
            SStr argumentList = matchCommandWithArgumentList[1].str();

            std::regex regexArgument("\\s*(" + patternValidName + "(" + patternAssignation + patternValidName + ")?)\\s*");

            auto argumentlistBegin = std::sregex_iterator(argumentList.begin(), argumentList.end(), regexArgument);
            auto argumentlistEnd = std::sregex_iterator();
        
            log("arguments");
            //VAL(std::distance(argumentlistBegin, argumentlistEnd))
        
            for (std::sregex_iterator i = argumentlistBegin; i != argumentlistEnd; ++i) {
                std::smatch argumentMatch = *i;                                                 
                SStr argumentStr = argumentMatch.str(); 

                std::regex regexPair("(" + patternValidName + ")(" + patternAssignation + "(" + patternValidName + "))?");

                std::smatch matchPair;
                std::regex_search(argumentStr, matchPair, regexPair);
                //bool isPair = !matchPair.empty();

                //VAL(matchPair[1].str())
                //VAL(matchPair[3].str())

                CommandArgument arg;
                arg.setName(matchPair[1].str());
                arg.setValue(matchPair[3].str());
                functor.getCommand().addArg(arg);
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

SStr CommandLine::autocomplete(const SStr &commandLine)
{
    return SStr(); // TODO : implement command autocomplete
}

void CommandLine::registerCommand(const SStr &commandName, CommandCallback callback)
{
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
        mUIText = UI::getInstance()->getUIBuilder().
        setLayout(UILayout::VERTICAL).
        setPosition(Vector2(-1.0f, -0.96f)).
        setLayer(0).
        setAdjustSizeToText(false).
        setTextSize(Vector2(0.04f, 0.04f)).
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