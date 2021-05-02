#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
class BatchesMap;
class Chunk;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;
class Vector3;
class LineRenderer;

class RenderEngine: public ObjectBase, public Singleton<RenderEngine> {

private:

	class LayerData : public ObjectBase {
	public:
		GENERATE_METADATA(LayerData);

	LayerData();
	virtual ~LayerData() override;

		 bool mSorted;
		 u32 mDynamicObjectsCount; // Non static objects count
		 u32 mSortCounter;
		 bool mVisible;
	};

	 BatchesMap* mBatchesMap;
	 BatchesMap* mBatchesMapScreenSpace;

	 LineRenderer* mLineRenderer;
	 LineRenderer* mLineRendererScreenSpace;

	 Camera* mCamera;
	 bool mCameraDirtyTranslation;
	
	 HashMap<u32, LayerData*>* mLayersData;
	 u32 mMaxLayers;
	 u32 mMaxLayersUsed;

	 f32 mMinChunkDrawDistance;
	 Array<Chunk*>* mChunks;

	 List<Renderer*>* mRenderersToFree;

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

public:

	GENERATE_METADATA(RenderEngine);

	RenderEngine();
	virtual ~RenderEngine() override;;

	GET_SET(Camera);

	GET(CameraDirtyTranslation);

	GET(LayersData);
	GET(MaxLayers);

	GET(MinChunkDrawDistance);

	void init(f32 sceneSize);
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer *renderer);
	Chunk* assignChunk(Renderer *renderer);
	void freeRenderer(Renderer *renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isAffectedByProjection = true, Vector4 color = Vector4(1,1,1,1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};
}