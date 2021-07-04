#pragma once

#include "Core/ObjectBase.hpp"

#include "Events/Event.hpp"

#define SEND_INPUT_EVENT(event) SEND_EVENT(nullptr, Input::getInstance(), event);

CLASS_EVENT(InputEventKeyEnter) {};
CLASS_EVENT(InputEventKeyEsc) {};
CLASS_EVENT(InputEventKeyDelete) {};
CLASS_EVENT(InputEventKeyBackspace) {};

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

CLASS_EVENT(InputEventMouseButtonReleased)
{
    PUB(Button, NONE, u32)
    PUB(Mods, NONE, u32)
};

// TODO : Make mouse repeat event?

CLASS_EVENT(InputEventKeyPressed)
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
