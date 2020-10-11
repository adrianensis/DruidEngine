#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace DE {

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
class Chunk;
class BatchesMap;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;
class Vector3;

class RenderEngine: public DE_Class, public Singleton<RenderEngine> {

private:
	Camera* mCamera;

	class LineRenderer: public DE_Class {
	public:
		DE_CLASS(LineRenderer, DE_Class);

		Array<f32>* mVertices; // A line is composed by 2 vertices.
		u32 mVAO;
		u32 mVBOPosition;
		u32 mEBO;
		bool mActive;
		bool mIsAffectedByProjection;
		f32 mSize;

		void init();
		void set(const Vector3 &start, const Vector3 &end);
		void bind(const Array<u32> *indices);
	};

	Shader* mShaderLine;
	Array<u32>* mLineRendererIndices; // [0,1]
	Array<LineRenderer*>* mLineRenderers;
	u32 mLineRenderersCount;

	List<Renderer*>* mRenderersToFree;

	Array<Chunk*>* mChunks;

	BatchesMap* mBatchesMap;
	BatchesMap* mBatchesMapNotAffectedByProjection;

	u32 mMaxLayersCount;
	u32 mMaxLayersUsed;
	u32 mMaxLayers = 10; // MOVE TO SETTINGS

	class LayerData: public DE_Class {
	public:
		DE_CLASS(LayerData, DE_Class);

		bool mSorted;
		u32 mDynamicObjectsCount; // Non static objects count
		u32 mSortCounter;
		bool mVisible;
	};

	HashMap<u32, LayerData*>* mLayersData;

	bool mCameraDirtyTranslation;
	bool mDebugColliders;

public:

	DE_CLASS(RenderEngine, DE_Class);

	void init(f32 sceneSize);
	void bind();
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer *renderer);
	Chunk* assignChunk(Renderer *renderer);
	void freeRenderer(Renderer *renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isAffectedByProjection = true, Vector4 color = Vector4(1,1,1,1));

	bool getDebugColliders() const {
		return mDebugColliders;
	};

	void setDebugColliders(bool debugColliders) {
		mDebugColliders = debugColliders;
	};

	Camera* getCamera() const {
		return mCamera;
	};

	void setCamera(Camera *newCamera) {
		mCamera = newCamera;
	};

	bool getCameraDirtyTranslation() const {
		return mCameraDirtyTranslation;
	};

	u32 getMaxLayers() const { return mMaxLayers; };

	HashMap<u32, LayerData*>* getLayersData() const { return mLayersData; };

	bool frustumTestSphere(const Vector3 &center, f32 radius);
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
