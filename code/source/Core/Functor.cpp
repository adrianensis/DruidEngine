#include "Functor.hpp"

namespace DE {

void Functor::execute() {
	if (mCallback)
		mCallback();
}

}
