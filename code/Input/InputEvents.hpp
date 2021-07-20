#pragma once

#include "Core/ObjectBase.hpp"

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
    PUB(ArrowButton, NONE, u32)
};

CLASS_EVENT(InputEventMouseMoved) {};

CLASS_EVENT(InputEventMouseButtonPressed)
{
    PUB(Button, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventMouseButtonHold)
{
    PUB(Button, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventMouseButtonReleased)
{
    PUB(Button, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventKeyPressed)
{
    PUB(Key, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventKeyHold)
{
    PUB(Key, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventKeyReleased)
{
    PUB(Key, NONE, u32)
    PUB(Mods, NONE, u32)
};

CLASS_EVENT(InputEventScroll)
{
    PUB(Scroll, NONE, f32)
};

CLASS_EVENT(InputEventChar)
{
    PUB(Char, NONE, char)
};
