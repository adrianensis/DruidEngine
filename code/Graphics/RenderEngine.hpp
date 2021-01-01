#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

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
class Chunk;
class BatchesMap;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;
class Vector3;

class RenderEngine: public DE_Class, public Singleton<RenderEngine> {

private:

	class LineRenderer: public DE_Class {
	public:
		DE_CLASS(LineRenderer)

		DE_M(Vertices, Array<f32>*) // A line is composed by 2 vertices.
		DE_M(VAO, u32)
		DE_M(VBOPosition, u32)
		DE_M(EBO, u32)
		DE_M(Active, bool)
		DE_M(IsAffectedByProjection, bool)
		DE_M(Size, f32)

		void init();
		void set(const Vector3 &start, const Vector3 &end);
		void bind(const Array<u32> *indices);
	};

	DE_M(ShaderLine, Shader*)
	DE_M(LineRendererIndices, Array<u32>*) // [0,1]
	DE_M(LineRenderers, Array<LineRenderer*>*)
	DE_M(LineRenderersCount, u32)
	DE_M(ThereAreActiveDebugRenderer, bool)

	DE_M(RenderersToFree, List<Renderer*>*)

	DE_M(Chunks, Array<Chunk*>*)

	DE_M(BatchesMap, BatchesMap*)
	DE_M(BatchesMapNotAffectedByProjection, BatchesMap*)

	DE_M(MaxLayersUsed, u32)

	class LayerData: public DE_Class {
	public:
		DE_CLASS(LayerData)

		DE_M(Sorted, bool)
		DE_M(DynamicObjectsCount, u32) // Non static objects count
		DE_M(SortCounter, u32)
		DE_M(Visible, bool)
	};

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

	using LayersDataMap = HashMap<u32, LayerData*>;

	DE_M_GET_SET(Camera, Camera*)
	DE_M_GET(CameraDirtyTranslation, bool)
	DE_M_GET(MaxLayers, u32)
	DE_M_GET(LayersData, LayersDataMap*)

public:

	DE_CLASS(RenderEngine)

	void init(f32 sceneSize);
	void bind();
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer *renderer);
	Chunk* assignChunk(Renderer *renderer);
	void freeRenderer(Renderer *renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isAffectedByProjection = true, Vector4 color = Vector4(1,1,1,1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
