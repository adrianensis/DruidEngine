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
class Vector3;

class RenderEngine : public DE_Class, public Singleton<RenderEngine>{
private:
	HashMap<Texture*, Batch*>* mBatches;
	Camera* mCamera;

	class LineRenderer : public DE_Class
  {
    public:
      DE_CLASS(LineRenderer, DE_Class);

      Array<f32>* mVertices; // A line is composed by 2 vertices.
      u32 mVAO;
      u32 mVBOPosition;
      u32 mEBO;
      bool mActive;

      void init();
			void set(const Vector3& start, const Vector3& end);
      void bind(const Array<u32>* indices);
  };

	  Shader* mShaderLine;
    Array<u32>* mLineRendererIndices; // [0,1]
    Array<LineRenderer*>* mLineRenderers;
    u32 mLineRenderersCount;

public:
	DE_CLASS(RenderEngine, DE_Class);

	void init();
	void bind();
	void update();
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer* renderer);
	void drawLine(const Vector3& start, const Vector3& end);

	void setCamera(Camera* camera);
	Camera* getCamera() const;
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
