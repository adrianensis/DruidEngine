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

CLASS(RenderEngine, ISubsystem), SINGLETON(RenderEngine)
{
private:
	CLASS_NESTED(LayerData, ObjectBase)
	{
		PUB(bool, Sorted, NONE)
		PUB(u32, DynamicObjectsCount, NONE) // Non static objects count
		PUB(u32, SortCounter, NONE)
		PUB(bool, Visible, NONE)

	public:
		LayerData();
	};

	PRI(BatchesMap, BatchesMap, NONE)
	PRI(BatchesMap, BatchesMapScreenSpace, NONE)

	PRI(LineRenderer *, LineRenderer, NONE)
	PRI(LineRenderer *, LineRendererScreenSpace, NONE)

	PRI(Camera *, Camera, GET_SET)
	PRI(bool, CameraDirtyTranslation, GET)

	PRI(SMap(u32, LayerData *), LayersData, GETREF_CONST_SET);
	PRI(u32, MaxLayers, GET)
	PRI(u32, MaxLayersUsed, NONE)

	PRI(f32, MinChunkDrawDistance, GET)
	PRI(SVec(Chunk *), Chunks, NONE)

	PRI(SLst(Renderer *), RenderersToFree, NONE)

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

public:
	void init(f32 sceneSize);
	void update();	  // render
	void updateDebug(); // debug render
	void terminate();

	VIR void addComponent(Component *component) OVR;
	Chunk *assignChunk(Renderer * renderer);
	void freeRenderer(Renderer * renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};