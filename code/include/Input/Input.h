#ifndef DE_INPUT_H_
#define DE_INPUT_H_

#include "DE_Class.h"

#include "RenderContext.h"

namespace DE {

class Input : public DE_Class{

public:

	DE_CLASS(Input, DE_Class);

	static void init();
	static void pollEvents();
	static bool isKeyPressed(u32 key);
};

} /* namespace DE */

#endif /* DE_INPUT_H_ */
