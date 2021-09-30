#pragma once

#include "Core/Core.hpp"

#include "Events/Event.hpp"

#define SEND_INPUT_EVENT(event) SEND_EVENT(nullptr, Input::getInstance(), event);

#define CLASS_EVENT_SPECIFIC_KEY(KeyName) CLASS_EVENT(InputEventKey ## KeyName) {};

CLASS_EVENT_SPECIFIC_KEY(Enter)
CLASS_EVENT_SPECIFIC_KEY(Esc)
CLASS_EVENT_SPECIFIC_KEY(Tab)
CLASS_EVENT_SPECIFIC_KEY(Delete)
CLASS_EVENT_SPECIFIC_KEY(Backspace)

CLASS_EVENT(InputEventKeyArrow)
{
    PUB(u32, ArrowButton, NONE)
};

CLASS_EVENT(InputEventMouseMoved) {};

CLASS_EVENT(InputEventMouseButtonPressed)
{
    PUB(u32, Button, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventMouseButtonHold)
{
    PUB(u32, Button, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventMouseButtonReleased)
{
    PUB(u32, Button, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyPressed)
{
    PUB(u32, Key, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyHold)
{
    PUB(u32, Key, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyReleased)
{
    PUB(u32, Key, NONE)
    PUB(u32, Mods, NONE)
};

CLASS_EVENT(InputEventScroll)
{
    PUB(f32, Scroll, NONE)
};

CLASS_EVENT(InputEventChar)
{
    PUB(char, Char, NONE)
};
