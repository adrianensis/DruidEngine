#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "List.h"
#include "Batch.h"
#include "RenderContext.h"
#include "Camera.h"

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
	void bind();
	void update();
	void render();
	void terminate();
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
