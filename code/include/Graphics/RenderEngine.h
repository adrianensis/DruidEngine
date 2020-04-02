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

	class LineRenderer : public DE_Class {
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

	class Chunk : public DE_Class {
		private:
			List<Renderer*>* mRenderers;
    public:
      DE_CLASS(Chunk, DE_Class);

			bool mThereAreNewRenderers;

			Vector3 mLeftTop;
			Vector3 mCenter;
			f32 mRadius;
			f32 mSize;

			bool mIsLoaded;

      void init();
			void set(const Vector3& leftTop, f32 size);
      void load(RenderEngine* renderEngine);
      void unload(RenderEngine* renderEngine);
			bool containsRenderer(const Renderer* renderer);
			void addRenderer(Renderer* renderer);
  };

	  Shader* mShaderLine;
    Array<u32>* mLineRendererIndices; // [0,1]
    Array<LineRenderer*>* mLineRenderers;
    u32 mLineRenderersCount;

		Array<Chunk*>* mChunks;

		u32 mMaxLayersCount;
		u32 mMaxLayersUsed;

		bool mCameraDirtyTranslation;

public:
	DE_CLASS(RenderEngine, DE_Class);

	void init();
	void bind();
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer* renderer);
	void drawLine(const Vector3& start, const Vector3& end);

	Camera* getCamera() const { return mCamera;};
	void setCamera(Camera* newCamera ) { mCamera = newCamera; };
	bool getCameraDirtyTranslation() const { return mCameraDirtyTranslation; };

	using MapBatches = HashMap<Texture*, Batch*>*;
	MapBatches getBatches() const { return mBatches; };
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
