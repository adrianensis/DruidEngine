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
		PUB_M(bool, Sorted, NONE)
		PUB_M(u32, DynamicObjectsCount, NONE) // Non static objects count
		PUB_M(u32, SortCounter, NONE)
		PUB_M(bool, Visible, NONE)

	PUB
		LayerData();
	};

	PRI_M(BatchesMap, BatchesMap, NONE)
	PRI_M(BatchesMap, BatchesMapScreenSpace, NONE)

	PRI_M(LineRenderer *, LineRenderer, NONE)
	PRI_M(LineRenderer *, LineRendererScreenSpace, NONE)

	PRI_M(Camera *, Camera, GET_SET)
	PRI_M(bool, CameraDirtyTranslation, GET)

	PRI_M(SMap(u32, LayerData), LayersData, GETREF_SET);
	PRI_M(u32, MaxLayers, GET)
	PRI_M(u32, MaxLayersUsed, NONE)

	PRI_M(f32, MinChunkDrawDistance, GET)
	PRI_M(SVec(Chunk *), Chunks, NONE)

	PRI_M(SLst(Renderer *), RenderersToFree, NONE)

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