#include "Event.hpp"

namespace DE {

Event::Event() : DE_Class() {
	mDelayAmount = 0;
	mDelayType = TimerDurationType::NONE;
	mInstigator = nullptr;
}

Event::~Event() {

}

} /* namespace DE */
