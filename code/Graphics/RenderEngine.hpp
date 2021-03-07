#pragma once

#include "Core/DE_Class.hpp"
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

class RenderEngine: public DE_Class, public Singleton<RenderEngine> {

private:

	class LayerData: public DE_Class {
	public:
		DE_CLASS_BODY(LayerData)

		DE_M(Sorted, bool)
		DE_M(DynamicObjectsCount, u32) // Non static objects count
		DE_M(SortCounter, u32)
		DE_M(Visible, bool)
	};

	DE_M(BatchesMap, BatchesMap*)
	DE_M(BatchesMapScreenSpace, BatchesMap*)

	DE_M(LineRenderer, LineRenderer*)
	DE_M(LineRendererScreenSpace, LineRenderer*)

	DE_M_GET_SET(Camera, Camera*)
	DE_M_GET(CameraDirtyTranslation, bool)
	
	using LayersDataMap = HashMap<u32, LayerData*>;
	DE_M_GET(LayersData, LayersDataMap*)
	DE_M_GET(MaxLayers, u32)
	DE_M(MaxLayersUsed, u32)

	DE_M_GET(MinChunkDrawDistance, f32)
	DE_M(Chunks, Array<Chunk*>*)

	DE_M(RenderersToFree, List<Renderer*>*)

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

public:

	DE_CLASS_BODY(RenderEngine)

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