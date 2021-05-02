#pragma once

#include "Core/ObjectBase.hpp"

#include "Events/Event.hpp"

namespace DE {

#define SEND_INPUT_EVENT(event) SEND_EVENT(nullptr, Input::getInstance(), event);

EVENT_DECLARATION_BEGIN(InputEventKeyEnter)
EVENT_DECLARATION_END(InputEventKeyEnter)

EVENT_DECLARATION_BEGIN(InputEventKeyEsc)
EVENT_DECLARATION_END(InputEventKeyEsc)

EVENT_DECLARATION_BEGIN(InputEventMouseMoved)
EVENT_DECLARATION_END(InputEventMouseMoved)

EVENT_DECLARATION_BEGIN(InputEventMouseButtonPressed)
u32 mButton = 0;
u32 mMods = 0;
EVENT_DECLARATION_END(InputEventMouseButtonPressed)

EVENT_DECLARATION_BEGIN(InputEventMouseButtonReleased)
u32 mButton = 0;
u32 mMods = 0;
EVENT_DECLARATION_END(InputEventMouseButtonReleased)

// TODO : Make mouse repeat event?

EVENT_DECLARATION_BEGIN(InputEventKeyPressed)
u32 mKey = 0;
u32 mMods = 0;
EVENT_DECLARATION_END(InputEventKeyPressed)

EVENT_DECLARATION_BEGIN(InputEventKeyReleased)
u32 mKey = 0;
u32 mMods = 0;
EVENT_DECLARATION_END(InputEventKeyReleased)

EVENT_DECLARATION_BEGIN(InputEventScroll)
u32 mScroll = 0;
EVENT_DECLARATION_END(InputEventScroll)

EVENT_DECLARATION_BEGIN(InputEventChar)
c8 mChar = 0;
EVENT_DECLARATION_END(InputEventChar)

}

