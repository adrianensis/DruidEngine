#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

#include <list>
#include <vector>
#include <map>

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

CLASS(RenderEngine, ObjectBase) , SINGLETON(RenderEngine) {

private:

	CLASS(LayerData, ObjectBase) {
		PUB(Sorted, NONE, bool)
		PUB(DynamicObjectsCount, NONE, u32) // Non static objects count
		PUB(SortCounter, NONE, u32)
		PUB(Visible, NONE, bool)

	public:
		LayerData();

	};

	PRI(BatchesMap, NONE, BatchesMap*)
	PRI(BatchesMapScreenSpace, NONE, BatchesMap*)

	PRI(LineRenderer, NONE, LineRenderer*)
	PRI(LineRendererScreenSpace, NONE, LineRenderer*)

	PRI(Camera, GET_SET, Camera*)
	PRI(CameraDirtyTranslation, GET, bool)
	
	PRI(LayersData, GETREF_CONST_SET, std::map<u32, LayerData*>);
	PRI(MaxLayers, GET, u32)
	PRI(MaxLayersUsed, NONE, u32)

	PRI(MinChunkDrawDistance, GET, f32)
	//PRI(Chunks, NONE, std::vector<Chunk*>)

	std::vector<Chunk*> mChunks;

	PRI(RenderersToFree, NONE, std::list<Renderer*>)

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

public:

	RenderEngine();
	virtual ~RenderEngine() override;

	void init(f32 sceneSize);
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer *renderer);
	Chunk* assignChunk(Renderer *renderer);
	void freeRenderer(Renderer *renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isWorldSpace = true, Vector4 color = Vector4(1,1,1,1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};