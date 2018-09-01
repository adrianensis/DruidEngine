#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "List.h"
#include "Batch.h"
#include "RenderContext.h"
#include "Camera.h"

namespace DE {

class RenderEngine {
private:
	List<Batch*>* mTextureBatches;
	RenderContext* mRenderContext;
	Camera* mCamera;

public:
	RenderEngine();
	virtual ~RenderEngine();

	void init();
	bool isClosed();
	void step();
	void terminate();
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
