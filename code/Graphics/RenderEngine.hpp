#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Graphics/Optimizations/BatchesMap.hpp"


class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
class BatchesMap;
class Chunk;
class Vector3;
class LineRenderer;

class RenderEngine : public ISubsystem, public Singleton<RenderEngine>
{
PRI
	GENERATE_METADATA(RenderEngine)

	class LayerData : public ObjectBase
	{
		GENERATE_METADATA(RenderEngine::LayerData)
		PUB bool mSorted = {};
		PUB u32 mDynamicObjectsCount = {}; // Non static objects count
		PUB u32 mSortCounter = {};
		PUB bool mVisible = {};

	PUB
		LayerData();
	};

	PRI BatchesMap mBatchesMap = {};
	PRI BatchesMap mBatchesMapScreenSpace = {};

	PRI LineRenderer * mLineRenderer = {};
	PRI LineRenderer * mLineRendererScreenSpace = {};

	PRI Camera * mCamera = {}; GET_SET(Camera)
	PRI bool mCameraDirtyTranslation = {}; GET(CameraDirtyTranslation)

	PRI std::map<u32, LayerData> mLayersData = {}; GETREF_SET(LayersData);
	PRI u32 mMaxLayers = {}; GET(MaxLayers)
	PRI u32 mMaxLayersUsed = {};

	PRI f32 mMinChunkDrawDistance = {}; GET(MinChunkDrawDistance)
	PRI std::vector<Chunk *> mChunks = {};

	PRI std::list<Renderer *> mRenderersToFree = {};

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

PUB
	void init(f32 sceneSize);
	void update();	  // render
	void terminate();

	virtual void addComponent(Component *component) override;
	Chunk *assignChunk(Renderer * renderer);
	void freeRenderer(Renderer * renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};