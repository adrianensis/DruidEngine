#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "List.h"

namespace DE {

class Batch;
class RenderContext;
class Camera;

class RenderEngine : public DE_Class{
private:
	List<Batch*>* mTextureBatches;
	RenderContext* mRenderContext;
	Camera* mCamera;

public:
	RenderEngine();
	~RenderEngine() override;

	void init();
	void bind();
	void update();
	void step(); // render
	void terminate();
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
