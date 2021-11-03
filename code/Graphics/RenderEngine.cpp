#include "Graphics/RenderEngine.hpp"
#include "Graphics/Optimizations/Batch.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Camera/Frustum.hpp"
#include "Graphics/Renderer.hpp"
#include "Log/Log.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Core/EngineConfig.hpp"
#include "Graphics/Optimizations/Chunk.hpp"
#include "Graphics/LineRenderer.hpp"
//#include "Profiler/Profiler.hpp"

RenderEngine::LayerData::LayerData()
{
	mSorted = false;
	mDynamicObjectsCount = 0;
	mSortCounter = 0;
	mVisible = true;
}

void RenderEngine::init(f32 sceneSize)
{
	TRACE()

	ISubsystem::init();

	REGISTER_COMPONENT_CLASS_IN_SUBSYSTEM(Renderer)

	mCameraDirtyTranslation = true;

	mLineRenderer = NEW(LineRenderer);
	mLineRenderer->init();

	mLineRendererScreenSpace = NEW(LineRenderer);
	mLineRendererScreenSpace->init();
	mLineRendererScreenSpace->mIsWorldSpace = false;

	// Static Chunks grid

	mMinChunkDrawDistance = EngineConfig::getInstance()->getConfig().at("scene").at("chunks").at("minChunkDrawDistance").get<f32>();

	f32 chunksGridSize = EngineConfig::getInstance()->getConfig().at("scene").at("chunks").at("gridSize").get<f32>();
	f32 chunksGridSizeHalf = chunksGridSize / 2.0f; // TODO : Make it power of 2!

	mChunks.reserve(chunksGridSize * chunksGridSize);

	f32 chunkSize = sceneSize / ((f32)chunksGridSize);

	for (i32 i = -chunksGridSizeHalf; i < chunksGridSizeHalf; ++i)
	{
		for (i32 j = chunksGridSizeHalf; j > -chunksGridSizeHalf; --j)
		{
			Chunk *chunk = NEW(Chunk);
			chunk->init();
			chunk->set(Vector2(i * chunkSize, j * chunkSize), chunkSize);

			mChunks.push_back(chunk);
		}
	}

	mBatchesMap.init();

	mBatchesMapScreenSpace.init();
	mBatchesMapScreenSpace.setIsWorldSpace(false);

	mMaxLayersUsed = 0;

	mMaxLayers = EngineConfig::getInstance()->getConfig().at("scene").at("maxLayers").get<u32>();
	FOR_RANGE(i, 0, mMaxLayers)
	{
		LayerData layerData;
		layerData.mSorted = EngineConfig::getInstance()->getConfig().at("scene").at("sortByYCoordinate").get<bool>();
		MAP_INSERT(mLayersData, i, layerData);
	}
}

void RenderEngine::freeRenderer(Renderer *renderer)
{
	//if(mRenderersToFree->find(renderer).isNull()){
	mRenderersToFree.push_back(renderer);
	//}
}

bool RenderEngine::frustumTestSphere(const Vector3 &center, f32 radius)
{
	return mCamera && mCamera->getFrustum() && mCamera->getFrustum()->testSphere(center, radius);
}

void RenderEngine::update()
{
	//PROFILER_TIMEMARK_START()

	if (mCamera)
	{
		if (mCamera->getFrustum())
		{
			mCamera->getFrustum()->build();
		}

		mCamera->calculateInverseMatrix();
	}

	renderBatches();
	swap();
	checkChunks();
	freeRenderersPendingtoFree();
	//PROFILER_TIMEMARK_END()
}

void RenderEngine::swap()
{
	//PROFILER_TIMEMARK_START()

	RenderContext::swap();

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::renderBatches()
{
	//PROFILER_TIMEMARK_START()

	FOR_RANGE(layer, 0, mMaxLayers)
	{
		if (mLayersData.at(layer).mVisible)
		{
			mBatchesMap.render(layer);
		}
	}

    mLineRenderer->render();

	FOR_RANGE(layer, 0, mMaxLayers)
	{
		//if(mLayersData->get(layer)->mVisible){
		mBatchesMapScreenSpace.render(layer);
		//}
	}

	mLineRendererScreenSpace->render();
    
	//PROFILER_TIMEMARK_END()
}

void RenderEngine::checkChunks()
{
	//PROFILER_TIMEMARK_START()

	FOR_ARRAY(i, mChunks)
	{
		Chunk *chunk = mChunks.at(i);

		f32 chunkToCameraDistance = chunk->getCenter().dst(mCamera->getGameObject()->getTransform()->getWorldPosition());
		bool chunkInDistance = chunkToCameraDistance <= mMinChunkDrawDistance;
		
		if (chunkInDistance)
		{
			chunk->load();
		}
		else
		{
			chunk->unload();
		}

		//if (chunk->getIsLoaded()) {
		chunk->update(&mBatchesMap);
		//}
	}

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::freeRenderersPendingtoFree()
{
	//PROFILER_TIMEMARK_START()

	LIST_DELETE_CONTENT(mRenderersToFree)

	mRenderersToFree.clear();

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::terminate()
{
	TRACE()

	if (mLineRenderer)
	{
		DELETE(mLineRenderer);
	}

	if (mLineRendererScreenSpace)
	{
		DELETE(mLineRendererScreenSpace);
	}

	LIST_DELETE_CONTENT(mChunks);

	Mesh::freeRectangle();

	LIST_DELETE_CONTENT(mRenderersToFree)
}

void RenderEngine::addComponent(Component *component)
{
	ISubsystem::addComponent(component);

	if(component->getClassId() == Renderer::getClassIdStatic())
	{
		Renderer *renderer = static_cast<Renderer*>(component);

		if (renderer->getIsWorldSpace())
		{
			Chunk *chunk = assignChunk(renderer);
			if (chunk)
			{
				chunk->addRenderer(renderer);
			}
			else
			{
				ASSERT_MSG(false, "Renderer can't find a chunk.")
			}
		}
		else
		{
			// UI Case!
			mBatchesMapScreenSpace.addRenderer(renderer);
		}

		mMaxLayersUsed = std::max(mMaxLayersUsed, renderer->getLayer() + 1);
	}
}

Chunk *RenderEngine::assignChunk(Renderer *renderer)
{
	//TRACE();
	bool found = false;
	Chunk *chunkTmp = nullptr;
	Chunk *chunkFound = nullptr;
	// FOR_ARRAY_COND(i, mChunks, !found) {
	for (i32 i = 0; (i < (i32)(mChunks.size())) && (!found); ++i)
	{
		// FOR_ARRAY(i, mChunks){
		chunkTmp = mChunks.at(i);
		if (chunkTmp->containsRenderer /*Sphere*/ (renderer))
		{
			renderer->setChunk(chunkTmp);

			// if(! renderer->isStatic()){
			found = true;
			chunkFound = chunkTmp;
			// }
		}
	}

	return chunkFound;
}

void RenderEngine::drawLine(const Vector3 &start, const Vector3 &end, f32 size /*= 1*/,
							bool isWorldSpace /*= true*/, Vector4 color /* = Vector4(1,1,1,1)*/)
{
	if (isWorldSpace)
	{
		mLineRenderer->add(start, end, color);
	}
	else
	{
		mLineRendererScreenSpace->add(start, end, color);
	}
}