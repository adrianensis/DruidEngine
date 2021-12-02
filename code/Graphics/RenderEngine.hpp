#pragma once

#include "Core/Core.hpp"
#include "Maths/Maths.hpp"
#include "Graphics/Optimizations/BatchesMap.hpp"
#include "Graphics/ShapeBatchRenderer.hpp"

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
class BatchesMap;
class Chunk;
class Vector3;

class RenderEngine : public ISubsystem, public Singleton<RenderEngine>
{
PRI
	GENERATE_METADATA(RenderEngine)

	PRI BatchesMap mBatchesMap;

	PRI ShapeBatchRendererMap mShapeBatchRendererMap;
	PRI ShapeBatchRendererMap mShapeBatchRendererMapScreenSpace;

	PRI Camera* mCamera = nullptr; GET_SET(Camera)
	PRI bool mCameraDirtyTranslation = false; GET(CameraDirtyTranslation)

	PRI f32 mMinChunkDrawDistance = 0.0f; GET(MinChunkDrawDistance)
	PRI std::vector<Chunk *> mChunks;

	void checkChunks();
	void renderBatches();
	void swap();

PUB
	void init(f32 sceneSize);
	void update();	  // render
	void terminate();

	virtual void addComponent(Component *component) override;
	Chunk *assignChunk(Renderer * renderer);
	void drawLine(const Line& line, f32 size = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};