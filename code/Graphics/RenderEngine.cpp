#include "Graphics/RenderEngine.hpp"
#include "Graphics/Batch.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Frustum.hpp"
#include "Graphics/Renderer.hpp"
#include "Log/Log.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Core/EngineConfig.hpp"
#include "Graphics/Chunk.hpp"
#include "Graphics/LineRenderer.hpp"
#include "Graphics/BatchesMap.hpp"
//#include "Profiler/Profiler.hpp"

RenderEngine::LayerData::LayerData(){
	mSorted = false;
	mDynamicObjectsCount = 0;
	mSortCounter = 0;
	mVisible = true;
}


RenderEngine::RenderEngine() {
	mCamera = nullptr;
	mBatchesMap = nullptr;
	mCameraDirtyTranslation = true;
}

RenderEngine::~RenderEngine() = default;

void RenderEngine::init(f32 sceneSize) {
	TRACE()

	mLineRenderer = new LineRenderer;
	mLineRenderer->init();

	mLineRendererScreenSpace = new LineRenderer;
	mLineRendererScreenSpace->init();
	mLineRendererScreenSpace->mIsWorldSpace = false;

	// Static Chunks grid

	mMinChunkDrawDistance = EngineConfig::getInstance()->getConfig().getF32("scene.chunks.minChunkDrawDistance");

	f32 chunksGridSize = EngineConfig::getInstance()->getConfig().getF32("scene.chunks.gridSize");
	f32 chunksGridSizeHalf = chunksGridSize / 2.0f; // TODO : Make it power of 2!

	mChunks.reserve(chunksGridSize * chunksGridSize);

	f32 chunkSize = sceneSize / ((f32) chunksGridSize);

	for (i32 i = -chunksGridSizeHalf; i < chunksGridSizeHalf; ++i) {
		for (i32 j = chunksGridSizeHalf; j > -chunksGridSizeHalf; --j) {
			Chunk* chunk = new Chunk;
			chunk->init();
			chunk->set(Vector2(i * chunkSize, j * chunkSize), chunkSize);

			mChunks.push_back(chunk);
		}
	}

	mBatchesMap = new BatchesMap;
	mBatchesMap->init();

	mBatchesMapScreenSpace = new BatchesMap;
	mBatchesMapScreenSpace->init();
	mBatchesMapScreenSpace->setIsWorldSpace(false);

	mMaxLayersUsed = 0;

	mMaxLayers = EngineConfig::getInstance()->getConfig().getU32("scene.maxLayers");
	FOR_RANGE(i, 0, mMaxLayers) {
		LayerData* layerData = new LayerData;
		layerData->mSorted = EngineConfig::getInstance()->getConfig().getBool("scene.sortByYCoordinate");
		MAP_INSERT(mLayersData, i, layerData);
	}
}

void RenderEngine::freeRenderer(Renderer *renderer) {
	//if(mRenderersToFree->find(renderer).isNull()){
		mRenderersToFree.push_back(renderer);
	//}
}

bool RenderEngine::frustumTestSphere(const Vector3 &center, f32 radius) {
	return mCamera && mCamera->getFrustum() && mCamera->getFrustum()->testSphere(center, radius);
}

void RenderEngine::step() {

	//PROFILER_TIMEMARK_START()

	if (mCamera) {
		if (mCamera->getFrustum()) {
			mCamera->getFrustum()->build();
		}

		mCamera->calculateInverseMatrix();
	}

	renderBatches();
	stepDebug();
	swap();
	checkChunks();
	freeRenderersPendingtoFree();
	//PROFILER_TIMEMARK_END()

}

void RenderEngine::swap() {
	//PROFILER_TIMEMARK_START()

	RenderContext::swap();

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::renderBatches() {
	//PROFILER_TIMEMARK_START()

	FOR_RANGE(layer, 0, mMaxLayers) {
		if(mLayersData.at(layer)->mVisible){
			mBatchesMap->render(layer);
		}
	}

	FOR_RANGE(layer, 0, mMaxLayers) {
		//if(mLayersData->get(layer)->mVisible){
			mBatchesMapScreenSpace->render(layer);
		//}
	}

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::checkChunks() {
	//PROFILER_TIMEMARK_START()

	FOR_ARRAY(i, mChunks) {
		Chunk* chunk = mChunks.at(i);

		f32 chunkToCameraDistance = chunk->getCenter().dst(mCamera->getGameObject()->getTransform()->getWorldPosition());
		bool chunkInDistance = chunkToCameraDistance <= mMinChunkDrawDistance; //chunkMinDrawDistance;

		if (chunkInDistance) {
			chunk->load();
		} else {
			chunk->unload();
		}

		//if (chunk->getIsLoaded()) {
			chunk->update(mBatchesMap);
		//}
	}

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::freeRenderersPendingtoFree() {
	//PROFILER_TIMEMARK_START()

	FOR_LIST(it, mRenderersToFree){
		delete *it;
	}

	mRenderersToFree.clear();

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::stepDebug() {
	//PROFILER_TIMEMARK_START()

	mLineRenderer->render();
	mLineRendererScreenSpace->render();

	//PROFILER_TIMEMARK_END()
}

void RenderEngine::terminate() {
	TRACE()

	delete mLineRenderer;
	delete mLineRendererScreenSpace;

	FOR_ARRAY(i, mChunks) {
		delete mChunks.at(i);
	}

	if(mBatchesMap){
		delete mBatchesMap;
	}

	if(mBatchesMapScreenSpace){
		delete mBatchesMapScreenSpace;
	}

	FOR_MAP(it, mLayersData) {
		delete it->second;
	}

	Mesh::freeRectangle();

	FOR_LIST(it, mRenderersToFree){
		delete *it;
	}
}

void RenderEngine::addRenderer(Renderer *renderer) {

	if (renderer->getIsWorldSpace()) {
		Chunk* chunk = assignChunk(renderer);
		if(chunk){
			chunk->addRenderer(renderer);
		} else {
			ASSERT_MSG(false, "Renderer can't find a chunk.")
		}
	} else {
		// UI Case!
		mBatchesMapScreenSpace->addRenderer(renderer);
	}

	mMaxLayersUsed = std::max(mMaxLayersUsed, renderer->getLayer() + 1);
}

Chunk* RenderEngine::assignChunk(Renderer *renderer) {
	//TRACE();
	bool found = false;
	Chunk* chunkTmp = nullptr;
	Chunk* chunkFound = nullptr;
	// FOR_ARRAY_COND(i, mChunks, !found) {
	for (i32 i = 0; (i < (i32)(mChunks.size())) && (!found); ++i) {
		// FOR_ARRAY(i, mChunks){
		chunkTmp = mChunks.at(i);
		if (chunkTmp->containsRenderer/*Sphere*/(renderer)) {
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
		bool isWorldSpace /*= true*/, Vector4 color /* = Vector4(1,1,1,1)*/) {
	
	if(isWorldSpace){
		mLineRenderer->add(start, end);
	} else{
		mLineRendererScreenSpace->add(start, end);
	}
}