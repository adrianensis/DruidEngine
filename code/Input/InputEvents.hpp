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
    PUB_M(u32, ArrowButton, NONE)
};

class InputEventMouseMoved: public Event
{
    GENERATE_METADATA(InputEventMouseMoved)
};

class InputEventMouseButtonPressed: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonPressed)
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventMouseButtonHold: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonHold)
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventMouseButtonReleased: public Event
{   
    GENERATE_METADATA(InputEventMouseButtonReleased)
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventKeyPressed: public Event
{   
    GENERATE_METADATA(InputEventKeyPressed)
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventKeyHold: public Event
{   
    GENERATE_METADATA(InputEventKeyHold)
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventKeyReleased: public Event
{   
    GENERATE_METADATA(InputEventKeyReleased)
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

class InputEventScroll: public Event
{   
    GENERATE_METADATA(InputEventScroll)
    PUB_M(f32, Scroll, NONE)
};

class InputEventChar: public Event
{   
    GENERATE_METADATA(InputEventChar)
    PUB_M(char, Char, NONE)
};
