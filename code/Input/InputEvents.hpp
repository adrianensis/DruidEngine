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
    PUB_M(u32, ArrowButton, NONE)
};

CLASS_EVENT(InputEventMouseMoved) {};

CLASS_EVENT(InputEventMouseButtonPressed)
{
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventMouseButtonHold)
{
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventMouseButtonReleased)
{
    PUB_M(u32, Button, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyPressed)
{
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyHold)
{
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventKeyReleased)
{
    PUB_M(u32, Key, NONE)
    PUB_M(u32, Mods, NONE)
};

CLASS_EVENT(InputEventScroll)
{
    PUB_M(f32, Scroll, NONE)
};

CLASS_EVENT(InputEventChar)
{
    PUB_M(char, Char, NONE)
};
