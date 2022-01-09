#pragma once

#include "Core/Core.hpp"
#include "Graphics/Optimizations/BatchesMap.hpp"
#include "Graphics/ShapeBatchRenderer.hpp"

class Camera;
class Renderer;
class Chunk;
class Vector3;

class RenderEngine : public ISubsystem, public Singleton<RenderEngine>
{
	GENERATE_METADATA(RenderEngine)

	PRI BatchesMap mBatchesMap;

	PRI ShapeBatchRendererMap mShapeBatchRendererMap;
	PRI ShapeBatchRendererMap mShapeBatchRendererMapScreenSpace;

	PRI Camera* mCamera = nullptr; GET_SET(Camera)
	PRI bool mCameraDirtyTranslation = false; GET(CameraDirtyTranslation)

	PRI f32 mMinChunkDrawDistance = 0.0f; GET(MinChunkDrawDistance)
	PRI std::vector<Chunk *> mChunks;

	PRI void checkChunks();
	PRI void renderBatches();
	PRI void swap();

	PUB void init(f32 sceneSize);
	PUB void update();	  // render
	PUB void terminate();

	PUB virtual void addComponent(Component *component) override;
	PUB Chunk *assignChunk(Renderer * renderer);
	PUB bool frustumTestSphere(const Vector3 &center, f32 radius);

	PUB void drawLine(const Line& line, f32 thickness = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
	PUB void drawRectangle2D(const Rectangle2D& rectangle, f32 thickness = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
	PUB void drawRectangle(const Rectangle& rectangle, f32 thickness = 1, bool isWorldSpace = true, Vector4 color = Vector4(1, 1, 1, 1));
};