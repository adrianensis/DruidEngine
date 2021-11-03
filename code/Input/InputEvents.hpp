#pragma once

#include "Core/Core.hpp"

#include "Events/Event.hpp"

#define SEND_INPUT_EVENT(event) SEND_EVENT(nullptr, Input::getInstance(), event);

#define CLASS_EVENT_SPECIFIC_KEY(KeyName) class InputEventKey##KeyName: public Event { GENERATE_METADATA(InputEventKey##KeyName) };

CLASS_EVENT_SPECIFIC_KEY(Enter)
CLASS_EVENT_SPECIFIC_KEY(Esc)
CLASS_EVENT_SPECIFIC_KEY(Tab)
CLASS_EVENT_SPECIFIC_KEY(Delete)
CLASS_EVENT_SPECIFIC_KEY(Backspace)

class InputEventKeyArrow: public Event
{   
    GENERATE_METADATA(InputEventKeyArrow)
    PUB u32 mArrowButton = {};
};

class InputEventMouseMoved: public Event
{
    GENERATE_METADATA(InputEventMouseMoved)
};

class InputEventMouseButtonPressed: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonPressed)
    PUB u32 mButton = {};
    PUB u32 mMods = {};
};

class InputEventMouseButtonHold: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonHold)
    PUB u32 mButton = {};
    PUB u32 mMods = {};
};

class InputEventMouseButtonReleased: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonReleased)
    PUB u32 mButton = {};
    PUB u32 mMods = {};
};

class InputEventKeyPressed: public Event
{   
    GENERATE_METADATA(InputEventKeyPressed)
    PUB u32 mKey = {};
    PUB u32 mMods = {};
};

class InputEventKeyHold: public Event
{   
    GENERATE_METADATA(InputEventKeyHold)
    PUB u32 mKey = {};
    PUB u32 mMods = {};
};

class InputEventKeyReleased: public Event
{   
    GENERATE_METADATA(InputEventKeyReleased)
    PUB u32 mKey = {};
    PUB u32 mMods = {};
};

class InputEventScroll: public Event
{   
    GENERATE_METADATA(InputEventScroll)
    PUB f32 mScroll = {};
};

class InputEventChar: public Event
{   
    GENERATE_METADATA(InputEventChar)
    PUB char mChar = {};
};
