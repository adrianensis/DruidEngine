#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "DE_Class.h"
#include "Component.h"

namespace DE {

class Renderer : public Component{
public:
	Renderer();
	~Renderer() override;
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
