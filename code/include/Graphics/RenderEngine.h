#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "List.h"
#include "Batch.h"
#include "RenderContext.h"
#include "Camera.h"
#include "DE_Class.h"

namespace DE {

class RenderEngine : public DE_Class{
private:
	List<Batch*>* mTextureBatches;
	RenderContext* mRenderContext;
	Camera* mCamera;

public:
	RenderEngine();
	~RenderEngine() override;

	void init();
	bool isClosed();
	void step();
	void terminate();
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
