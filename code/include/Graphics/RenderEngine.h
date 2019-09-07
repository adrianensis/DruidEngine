#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "DE_Class.h"

namespace DE {

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
template <class T> class List;
template <class K, class V> class HashMap;

class RenderEngine : public DE_Class{
private:
	HashMap<Texture*, Batch*>* mBatches;
	Camera* mCamera;

public:
	RenderEngine();
	~RenderEngine() override;

	void init();
	void bind();
	void update();
	void step(); // render
	void terminate();

	void addRenderer(Renderer* renderer);

	void setCamera(Camera* camera);
	Camera* getCamera() const;
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
