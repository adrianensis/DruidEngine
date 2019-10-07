#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "DE_Class.h"
#include "Singleton.h"
#include "Vector3.h"

namespace DE {

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
template <class T> class List;
template <class T> class Array;
template <class K, class V> class HashMap;

class RenderEngine : public DE_Class, public Singleton<RenderEngine>{
private:
	HashMap<Texture*, Batch*>* mBatches;
	Camera* mCamera;

public:
	DE_CLASS(RenderEngine, DE_Class);

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
