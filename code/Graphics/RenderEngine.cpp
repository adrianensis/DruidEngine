#include "Graphics/RenderEngine.hpp"
#include "Memory/Memory.hpp"
#include "Graphics/Batch.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Frustum.hpp"
#include "Graphics/Renderer.hpp"
#include "Containers/List.hpp"
#include "Containers/Array.hpp"
#include "Containers/HashMap.hpp"
#include "Log/Log.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Config/EngineConfig.hpp"
#include "Graphics/Chunk.hpp"
#include "UI/UI.hpp"
#include "Profiler/Profiler.hpp"

namespace DE {

// ---------------------------------------------------------------------------

RenderEngine::LineRenderer::LineRenderer() : DE_Class() {
	mVertices = nullptr;
	mActive = false;
}

RenderEngine::LineRenderer::~LineRenderer() {
	DE_FREE(mVertices);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::init() {
	//TRACE();

	if (!mVertices) {
		mVertices = DE_NEW<Array<f32>>();
		mVertices->init(2 * 3); // 1 vertex = 3 floats
	}

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::set(const Vector3 &start, const Vector3 &end) {

	mVertices->set(0, start.x);
	mVertices->set(1, start.y);
	mVertices->set(2, start.z);
	mVertices->set(3, end.x);
	mVertices->set(4, end.y);
	mVertices->set(5, end.z);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::bind(const Array<u32> *indices) {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(3, 0);
	mEBO = RenderContext::createEBO();

	RenderContext::setDataVBO(mVBOPosition, mVertices);
	RenderContext::setDataEBO(mEBO, indices);

	RenderContext::enableVAO(mVAO);
}

// ---------------------------------------------------------------------------

RenderEngine::LayerData::LayerData() : DE_Class() {
	mSorted = false;
	mDynamicObjectsCount = 0;
	mSortCounter = 0;
	mVisible = true;
}

// ---------------------------------------------------------------------------

RenderEngine::LayerData::~LayerData() = default;

// ---------------------------------------------------------------------------

RenderEngine::RenderEngine() : DE_Class(), Singleton<RenderEngine>() {
	mCamera = nullptr;
	mLineRenderers = nullptr;
	mLineRendererIndices = nullptr;
	mShaderLine = nullptr;
	mBatchesMap = nullptr;
	mCameraDirtyTranslation = true;
}

RenderEngine::~RenderEngine() = default;

// ---------------------------------------------------------------------------

void RenderEngine::init(f32 sceneSize) {
	DE_TRACE()

	mLineRenderers = DE_NEW<Array<LineRenderer*>>();
	mLineRendererIndices = DE_NEW<Array<u32>>();
	mShaderLine = DE_NEW<Shader>();

	// Line Renderers

	mLineRenderersCount = EngineConfig::getInstance()->getF32("line.renderers.count");
	mLineRenderers->init(mLineRenderersCount);

	mLineRendererIndices->init(2);
	mLineRendererIndices->set(0, 0.0f);
	mLineRendererIndices->set(1, 1.0f);

	mThereAreActiveDebugRenderer = false;

	mShaderLine->initDebug();

	FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount) {
		LineRenderer* lineRenderer = DE_NEW<LineRenderer>();
		lineRenderer->init();
		lineRenderer->mActive = false;

		mLineRenderers->set(i, lineRenderer);
	}

	mRenderersToFree = DE_NEW<List<Renderer*>>();
	mRenderersToFree->init();

	// Static Chunks grid

	f32 chunksGridSize = EngineConfig::getInstance()->getF32("scene.chunks.gridSize");
	f32 chunksGridSizeHalf = chunksGridSize / 2.0f;

	mChunks = DE_NEW<Array<Chunk*>>();
	mChunks->init(chunksGridSize * chunksGridSize); // TODO : define how many chunks to create. Move to EngineConfig.

	f32 chunkSize = sceneSize / ((f32) chunksGridSize);

	u32 count = 0;
	for (i32 i = -chunksGridSizeHalf; i < chunksGridSizeHalf; ++i) {
		for (i32 j = chunksGridSizeHalf; j > -chunksGridSizeHalf; --j) {
			Chunk* chunk = DE_NEW<Chunk>();
			chunk->init();
			chunk->set(Vector2(i * chunkSize, j * chunkSize), chunkSize);

			mChunks->set(count, chunk);
			count++;
		}
	}

	mBatchesMap = DE_NEW<BatchesMap>();
	mBatchesMap->init();

	mBatchesMapNotAffectedByProjection = DE_NEW<BatchesMap>();
	mBatchesMapNotAffectedByProjection->init();

	mMaxLayersUsed = 0;

	mLayersData = DE_NEW<HashMap<u32, LayerData*>>();
	mLayersData->init();

	mMaxLayers = EngineConfig::getInstance()->getU32("scene.maxLayers");
	FOR_RANGE(i, 0, mMaxLayers) {
		LayerData* layerData = DE_NEW<LayerData>();
		layerData->mSorted = EngineConfig::getInstance()->getBool("scene.sortByYCoordinate");
		mLayersData->set(i, layerData);
	}
}

void RenderEngine::freeRenderer(Renderer *renderer) {
	if(mRenderersToFree->find(renderer).isNull()){
		mRenderersToFree->pushBack(renderer);
	}
}

// ---------------------------------------------------------------------------

bool RenderEngine::frustumTestSphere(const Vector3 &center, f32 radius) {
	return mCamera && mCamera->getFrustum() && mCamera->getFrustum()->testSphere(center, radius);
}

// ---------------------------------------------------------------------------

void RenderEngine::step() {

	DE_TIMEMARK_START()

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
	DE_TIMEMARK_END()

}

// ---------------------------------------------------------------------------

void RenderEngine::swap() {
	DE_TIMEMARK_START()

	RenderContext::swap();

	DE_TIMEMARK_END()
}

void RenderEngine::renderBatches() {
	DE_TIMEMARK_START()

	u32 drawCallCounter = 0;

	FOR_RANGE(layer, 0, mMaxLayers) {

		if(mLayersData->get(layer)->mVisible){
			drawCallCounter += mBatchesMap->render(layer);
		}
	}

	FOR_RANGE(layer, 0, mMaxLayers) {
		//if(mLayersData->get(layer)->mVisible){
		drawCallCounter += mBatchesMapNotAffectedByProjection->render(layer);
		//}
	}

	// VAR(u32,drawCallCounter);

	DE_TIMEMARK_END()
}

void RenderEngine::checkChunks() {
	DE_TIMEMARK_START()

	FOR_ARRAY(i, mChunks) {
		Chunk* chunk = mChunks->get(i);

		bool chunkInCameraView = frustumTestSphere(chunk->getCenter(), chunk->getRadius() * 2);

		if (chunkInCameraView) {
			chunk->load();
		} else {
			chunk->unload();
		}

		if (chunk->getIsLoaded()) {
			chunk->update(mBatchesMap);
		}
	}

	DE_TIMEMARK_END()
}

void RenderEngine::freeRenderersPendingtoFree() {
	DE_TIMEMARK_START()

	FOR_LIST(it, mRenderersToFree){
		DE_FREE(it.get());
	}

	mRenderersToFree->clear();

	DE_TIMEMARK_END()
}

// ---------------------------------------------------------------------------

void RenderEngine::stepDebug() {
	DE_TIMEMARK_START()

	mShaderLine->use();

	const Matrix4& projectionMatrix = getCamera()->getProjectionMatrix();
	const Matrix4& viewTranslationMatrix = getCamera()->getViewTranslationMatrix();
	const Matrix4& viewRotationMatrix = getCamera()->getViewRotationMatrix();

	FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount) {
		LineRenderer* lineRenderer = mLineRenderers->get(i);

		if (lineRenderer->mActive) {

			if (lineRenderer->mIsAffectedByProjection) {
				mShaderLine->addMatrix(projectionMatrix, "projectionMatrix");
				mShaderLine->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
				mShaderLine->addMatrix(viewRotationMatrix, "viewRotationMatrix");
			} else {
				mShaderLine->addMatrix(Matrix4::getIdentity(), "projectionMatrix");
				mShaderLine->addMatrix(Matrix4::getIdentity(), "viewTranslationMatrix");
				mShaderLine->addMatrix(Matrix4::getIdentity(), "viewRotationMatrix");
			}

			lineRenderer->bind(mLineRendererIndices);

			glLineWidth(lineRenderer->mSize);
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

			RenderContext::enableVAO(0);

			lineRenderer->mActive = false;
		}
	}

	mThereAreActiveDebugRenderer = false;

	DE_TIMEMARK_END()
}

// ---------------------------------------------------------------------------

void RenderEngine::bind() {
	// FOR_ARRAY(i, mChunks){
	//   mChunks->get(i)->bind();
	// }
	//
	// mScreenChunk->bind();
}

// ---------------------------------------------------------------------------

void RenderEngine::terminate() {
	DE_TRACE()

	if(mLineRendererIndices){
		DE_FREE(mLineRendererIndices);
	}

	if(mShaderLine) {
		DE_FREE(mShaderLine);
	}

	if(mLineRenderers){
		FOR_ARRAY(i, mLineRenderers){
			DE_FREE(mLineRenderers->get(i));
		}

		DE_FREE(mLineRenderers);
	}

	if(mChunks){
		FOR_ARRAY(i, mChunks) {
			DE_FREE(mChunks->get(i));
		}

		DE_FREE(mChunks);
	}


	if(mBatchesMap){
		DE_FREE(mBatchesMap);
	}

	if(mBatchesMapNotAffectedByProjection){
		DE_FREE(mBatchesMapNotAffectedByProjection);
	}

	if(mLayersData){
		FOR_LIST(it, mLayersData->getValues()) {
			DE_FREE(it.get());
		}

		DE_FREE(mLayersData);
	}

	//DE_FREE(Mesh::getRectangle());

	Mesh::freeRectangle();

	if(mRenderersToFree){
		FOR_LIST(it, mRenderersToFree){
			DE_FREE(it.get());
		}

		DE_FREE(mRenderersToFree);
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::addRenderer(Renderer *renderer) {

	if (renderer->getIsAffectedByProjection()) {
		Chunk* chunk = assignChunk(renderer);
		if(chunk){
			chunk->addRenderer(renderer);
		} else {
			//DE_ASSERT(false, "Renderer can't find a chunk.")
		}
	} else {
		// UI Case!
		mBatchesMapNotAffectedByProjection->addRenderer(renderer);
	}

	mMaxLayersUsed = std::max(mMaxLayersUsed, renderer->getLayer() + 1);
}

// ---------------------------------------------------------------------------

Chunk* RenderEngine::assignChunk(Renderer *renderer) {
	//TRACE();
	bool found = false;
	Chunk* chunkTmp = nullptr;
	Chunk* chunkFound = nullptr;
	FOR_ARRAY_COND(i, mChunks, !found) {
		// FOR_ARRAY(i, mChunks){
		chunkTmp = mChunks->get(i);
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

// ---------------------------------------------------------------------------

void RenderEngine::drawLine(const Vector3 &start, const Vector3 &end, f32 size /*= 1*/,
		bool isAffectedByProjection /*= true*/, Vector4 color /* = Vector4(1,1,1,1)*/) {
	bool found = false;

	FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount && !found) {
		LineRenderer* lineRenderer = mLineRenderers->get(i);

		if (!lineRenderer->mActive) {
			found = true;
			lineRenderer->init();
			lineRenderer->set(start, end);
			lineRenderer->mActive = true;
			lineRenderer->mIsAffectedByProjection = isAffectedByProjection;
			lineRenderer->mSize = size;

			mThereAreActiveDebugRenderer = true;
		}
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
