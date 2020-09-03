#include "RenderEngine.hpp"
#include "Memory.hpp"
#include "Batch.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "RenderContext.hpp"
#include "Camera.hpp"
#include "Frustum.hpp"
#include "Renderer.hpp"
#include "List.hpp"
#include "Array.hpp"
#include "HashMap.hpp"
#include "Log.hpp"
#include "Shader.hpp"
#include "Vector3.hpp"
#include "MathUtils.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Settings.hpp"
#include "Chunk.hpp"
#include "UI.hpp"

namespace DE {

// ---------------------------------------------------------------------------

RenderEngine::LineRenderer::LineRenderer() : DE_Class() {
	mVertices = nullptr;
	mActive = false;
}

RenderEngine::LineRenderer::~LineRenderer() {
	Memory::free<Array<f32>>(mVertices);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::init() {
	//TRACE();

	if (!mVertices) {
		mVertices = Memory::allocate<Array<f32>>();
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
	mVBOPosition = RenderContext::createVBO(mVertices, 3, 0);
	mEBO = RenderContext::createEBO(indices);

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

RenderEngine::RenderEngine() : DE_Class(), Singleton() {
	mCamera = nullptr;
	mLineRenderers = nullptr;
	mLineRendererIndices = nullptr;
	mShaderLine = nullptr;
}

RenderEngine::~RenderEngine() = default;

// ---------------------------------------------------------------------------

void RenderEngine::init(f32 sceneSize) {
	TRACE();

	mLineRenderers = Memory::allocate<Array<LineRenderer*>>();
	mLineRendererIndices = Memory::allocate<Array<u32>>();
	mShaderLine = Memory::allocate<Shader>();

	// Line Renderers

	mLineRenderersCount = Settings::getInstance()->getF32("line.renderers.count");
	mLineRenderers->init(mLineRenderersCount);

	mLineRendererIndices->init(2);
	mLineRendererIndices->set(0, 0.0f);
	mLineRendererIndices->set(1, 1.0f);

	mShaderLine->initDebug();

	FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount) {
		LineRenderer* lineRenderer = Memory::allocate<LineRenderer>();
		lineRenderer->init();
		lineRenderer->mActive = false;

		mLineRenderers->set(i, lineRenderer);
	}

	mRenderersToFree = Memory::allocate<List<Renderer*>>();
	mRenderersToFree->init();

	// Static Chunks grid

	f32 chunksGridSize = Settings::getInstance()->getF32("scene.chunks.count");
	f32 chunksGridSizeHalf = chunksGridSize / 2.0f;

	mChunks = Memory::allocate<Array<Chunk*>>();
	mChunks->init(chunksGridSize * chunksGridSize); // TODO : define how many chunks to create. Move to Settings.

	f32 chunkSize = sceneSize / ((f32) chunksGridSize);

	u32 count = 0;
	for (i32 i = -chunksGridSizeHalf; i < chunksGridSizeHalf; ++i) {
		for (i32 j = chunksGridSizeHalf; j > -chunksGridSizeHalf; --j) {
			Chunk* chunk = Memory::allocate<Chunk>();
			chunk->init();
			chunk->set(Vector2(i * chunkSize, j * chunkSize), chunkSize);

			mChunks->set(count, chunk);
			count++;
		}
	}

	mBatchesMap = Memory::allocate<BatchesMap>();
	mBatchesMap->init();

	mBatchesMapNotAffectedByProjection = Memory::allocate<BatchesMap>();
	mBatchesMapNotAffectedByProjection->init();

	mMaxLayersCount = 10;
	mMaxLayersUsed = 0;

	mLayersData = Memory::allocate<HashMap<u32, LayerData*>>();
	mLayersData->init();

	mMaxLayers = Settings::getInstance()->getU32("scene.maxLayers");
	FOR_RANGE(i, 0, mMaxLayers) {
		LayerData* layerData = Memory::allocate<LayerData>();
		layerData->mSorted = Settings::getInstance()->getBool("scene.sortByYCoordinate");
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

	if (mCamera) {
		if (mCamera->getFrustum()) {
			mCamera->getFrustum()->build();
		}

		mCamera->calculateInverseMatrix();
	}

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

	stepDebug();

	RenderContext::swap();

	//check Chunks
	FOR_ARRAY(i, mChunks) {
		Chunk* chunk = mChunks->get(i);

		bool chunkInCameraView = frustumTestSphere(chunk->mCenter, chunk->mRadius * 2);

		if (chunkInCameraView) {
			chunk->load();
		} else {
			chunk->unload();
		}

		if (chunk->isLoaded()) {
			chunk->update(mBatchesMap);
		}
	}

	FOR_LIST(it, mRenderersToFree){
		Memory::free<Renderer>(it.get());
	}

	mRenderersToFree->clear();

} // ---------------------------------------------------------------------------

void RenderEngine::stepDebug() {

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
	TRACE();

	if(mLineRendererIndices){
		Memory::free<Array<u32>>(mLineRendererIndices);
	}

	if(mShaderLine) {
		Memory::free<Shader>(mShaderLine);
	}

	if(mLineRenderers){
		FOR_ARRAY(i, mLineRenderers){
			Memory::free<LineRenderer>(mLineRenderers->get(i));
		}

		Memory::free<Array<LineRenderer*>>(mLineRenderers);
	}

	if(mChunks){
		FOR_ARRAY(i, mChunks) {
			Memory::free<Chunk>(mChunks->get(i));
		}

		Memory::free<Array<Chunk*>>(mChunks);
	}


	if(mBatchesMap){
		Memory::free<BatchesMap>(mBatchesMap);
	}

	if(mBatchesMapNotAffectedByProjection){
		Memory::free<BatchesMap>(mBatchesMapNotAffectedByProjection);
	}

	if(mLayersData){
		FOR_LIST(it, mLayersData->getValues()) {
			Memory::free<LayerData>(it.get());
		}

		Memory::free<HashMap<u32, LayerData*>>(mLayersData);
	}

	//Memory::free<Mesh>(Mesh::getRectangle());

	Mesh::freeRectangle();

	if(mRenderersToFree){
		FOR_LIST(it, mRenderersToFree){
			Memory::free<Renderer>(it.get());
		}

		VAR(u32, mRenderersToFree->getLength())

		Memory::free<List<Renderer*>>(mRenderersToFree);
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::addRenderer(Renderer *renderer) {

	if (renderer->isAffectedByProjection()) {
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
		}
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
