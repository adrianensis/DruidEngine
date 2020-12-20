#ifndef DE_INPUTEVENTS_H_
#define DE_INPUTEVENTS_H_

#include "DE_Class.hpp"

#include "Event.hpp"

namespace DE {

#define DE_SEND_INPUT_EVENT(event) DE_SEND_EVENT(nullptr, Input::getInstance(), event);

DE_EVENT_DECLARATION_BEGIN(InputEventKeyEnter)
DE_EVENT_DECLARATION_END(InputEventKeyEnter)

DE_EVENT_DECLARATION_BEGIN(InputEventKeyEsc)
DE_EVENT_DECLARATION_END(InputEventKeyEsc)

DE_EVENT_DECLARATION_BEGIN(InputEventMouseButtonPressed)
u32 mButton = 0;
u32 mMods = 0;
DE_EVENT_DECLARATION_END(InputEventMouseButtonPressed)

DE_EVENT_DECLARATION_BEGIN(InputEventMouseButtonReleased)
u32 mButton = 0;
u32 mMods = 0;
DE_EVENT_DECLARATION_END(InputEventMouseButtonReleased)

// TODO : Make mouse repeat event?

DE_EVENT_DECLARATION_BEGIN(InputEventKeyPressed)
u32 mKey = 0;
u32 mMods = 0;
DE_EVENT_DECLARATION_END(InputEventKeyPressed)

DE_EVENT_DECLARATION_BEGIN(InputEventKeyReleased)
u32 mKey = 0;
u32 mMods = 0;
DE_EVENT_DECLARATION_END(InputEventKeyReleased)

DE_EVENT_DECLARATION_BEGIN(InputEventScroll)
u32 mScroll = 0;
DE_EVENT_DECLARATION_END(InputEventScroll)

DE_EVENT_DECLARATION_BEGIN(InputEventChar)
c8 mChar = 0;
DE_EVENT_DECLARATION_END(InputEventChar)

} /* namespace DE */

#endif /* DE_INPUTEVENTS_H_ */
