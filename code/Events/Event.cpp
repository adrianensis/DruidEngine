#include "Events/Event.hpp"

namespace DE {

Event::Event() : ObjectBase() {
	mDelayAmount = 0;
	mDelayType = TimerDurationType::NONE;
	mInstigator = nullptr;
}

Event::~Event() {

}

}
