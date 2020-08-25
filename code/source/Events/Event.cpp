#include "Event.hpp"

namespace DE {

Event::Event() : DE_Class() {
	mDelayAmount = 0;
	mDelayType = EventDelay::NONE;
	mSender = nullptr;
}

Event::~Event() {

}

} /* namespace DE */
