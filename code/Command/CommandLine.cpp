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

    SUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this, [this](const Event *event)
	{
        if(mUIText && mUIText->isActive())
        {
            const InputEventChar *e = (InputEventChar*) event;
            char c = e->mChar;
            mBuffer.push_back(c);
            //VAR(mBuffer)
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this, [this](const Event *event)
	{
        if(mUIText && mUIText->isActive())
        {
            execute(mBuffer);
            mBuffer.clear();
            //VAR(mBuffer)
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [this](const Event *event)
	{
        if(mUIText && mUIText->isActive())
        {
            mBuffer.clear();
            //VAR(mBuffer)
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
        if(mUIText && mUIText->isActive())
        {
            if(!mBuffer.empty())
            {
                mBuffer.pop_back();
            }
            
            //VAR(mBuffer)
        }
	});

    SUBSCRIBE_TO_EVENT(InputEventKeyArrow, nullptr, this, [this](const Event *event)
	{
        if(mUIText && mUIText->isActive())
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
}

void CommandLine::update()
{
    if(mUIText && mUIText->isActive())
    {
        mUIText->setText("> " + mBuffer);
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
        ECHO("command: " + commandName)

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
        
            ECHO("Arguments")
            VAL(std::distance(argumentlistBegin, argumentlistEnd))
        
            for (std::sregex_iterator i = argumentlistBegin; i != argumentlistEnd; ++i) {
                std::smatch argumentMatch = *i;                                                 
                SStr argumentStr = argumentMatch.str(); 

                std::regex regexPair("(" + patternValidName + ")(" + patternAssignation + "(" + patternValidName + "))?");

                std::smatch matchPair;
                std::regex_search(argumentStr, matchPair, regexPair);
                bool isPair = !matchPair.empty();

                VAL(matchPair[1].str())
                VAL(matchPair[3].str())

                CommandArgument arg;
                arg.setName(matchPair[1].str());
                arg.setValue(matchPair[3].str());
                functor.getCommand().addArg(arg);
            }   
        }
        
        functor.execute();
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
    if(!mUIText)
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
    }
}

void CommandLine::close()
{
    if(mUIText && mUIText->isActive())
    {
        mUIText->setIsActive(false);
    }
}