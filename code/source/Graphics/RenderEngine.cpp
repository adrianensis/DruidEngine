#include "RenderEngine.h"
#include "Memory.h"
#include "Batch.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Renderer.h"
#include "List.h"
#include "Array.h"
#include "HashMap.h"
#include "Log.h"
#include "Shader.h"
#include "Vector3.h"
#include "MathUtils.h"

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

  if(!mVertices){
    mVertices = Memory::allocate<Array<f32>>();
    mVertices->init(2*3); // 1 vertex = 3 floats
  }

  glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::set(const Vector3& start, const Vector3& end){

  mVertices->set(0,start.x);
  mVertices->set(1,start.y);
  mVertices->set(2,start.z);
  mVertices->set(3,end.x);
  mVertices->set(4,end.y);
  mVertices->set(5,end.z);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::bind(const Array<u32>* indices) {
  mVAO = RenderContext::createVAO();
  mVBOPosition = RenderContext::createVBO(mVertices, 3, 0);
  mEBO = RenderContext::createEBO(indices);

  RenderContext::enableVAO(mVAO);
}

// ---------------------------------------------------------------------------

RenderEngine::RenderEngine() : DE_Class(), Singleton() {
	mBatches = nullptr;
	mCamera = nullptr;
  mLineRenderers = nullptr;
  mLineRendererIndices = nullptr;
  mShaderLine = nullptr;
  mLineRenderersCount = 50;
}

RenderEngine::~RenderEngine() = default;

// ---------------------------------------------------------------------------

void RenderEngine::init() {
	TRACE();

	mBatches = Memory::allocate<HashMap<Texture*, Batch*>>();
  mLineRenderers = Memory::allocate<Array<LineRenderer*>>();
  mLineRendererIndices = Memory::allocate<Array<u32>>();
  mShaderLine = Memory::allocate<Shader>();

  RenderContext::init();
  mBatches->init();

  mLineRenderers->init(mLineRenderersCount);

  mLineRendererIndices->init(2);
  mLineRendererIndices->set(0,0.0f);
  mLineRendererIndices->set(1,1.0f);

  mShaderLine->initDebug();

  FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount){
    LineRenderer* lineRenderer = Memory::allocate<LineRenderer>();
    lineRenderer->init();
    lineRenderer->mActive = false;

    mLineRenderers->set(i,lineRenderer);
  }

  mMaxLayersCount = 10;
  mMaxLayersUsed = 1;
}

// ---------------------------------------------------------------------------

void RenderEngine::step() {

  FOR_RANGE(layer, 0, mMaxLayersCount){
  	FOR_LIST(it, mBatches->getValues()){
  		it.get()->render(layer-1);
  	}
	}

  //stepDebug();

	RenderContext::swap();

}// ---------------------------------------------------------------------------

void RenderEngine::stepDebug() {

  mShaderLine->use();

  const Matrix4& projectionMatrix = getCamera()->getProjectionMatrix();
  const Matrix4& viewTranslationMatrix = getCamera()->getViewTranslationMatrix();
  const Matrix4& viewRotationMatrix = getCamera()->getViewRotationMatrix();

  mShaderLine->addMatrix(projectionMatrix, "projectionMatrix");
  mShaderLine->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
  mShaderLine->addMatrix(viewRotationMatrix, "viewRotationMatrix");

  FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount){
    LineRenderer* lineRenderer = mLineRenderers->get(i);

    if(lineRenderer->mActive){
      lineRenderer->bind(mLineRendererIndices);

      glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

      RenderContext::enableVAO(0);

      lineRenderer->mActive = false;
    }
  }
}

// ---------------------------------------------------------------------------

void RenderEngine::bind() {

	FOR_LIST(it, mBatches->getValues()){
		it.get()->bind();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::update() {

	FOR_LIST(it, mBatches->getValues()){
		it.get()->update();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::terminate() {

	FOR_LIST(it, mBatches->getValues()){
		Memory::free<Batch>(it.get());
	}

	Memory::free<HashMap<Texture*, Batch*>>(mBatches);

  Memory::free<Array<u32>>(mLineRendererIndices);
  Memory::free<Shader>(mShaderLine);

  FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount){
    Memory::free<LineRenderer>(mLineRenderers->get(i));
  }

  Memory::free<Array<LineRenderer*>>(mLineRenderers);

	RenderContext::terminate();
}

// ---------------------------------------------------------------------------

void RenderEngine::addRenderer(Renderer* renderer) {
	Texture* texture = renderer->getMaterial()->getTexture();

	if( ! mBatches->contains(texture)) {

		Batch* batch = Memory::allocate<Batch>();
		batch->init(this, renderer->getMesh(), renderer->getMaterial());

		mBatches->set(texture, batch);
	}

	mBatches->get(texture)->addRenderer(renderer);

  mMaxLayersUsed = std::max(mMaxLayersUsed, renderer->getLayer()+1);
}
// ---------------------------------------------------------------------------

void RenderEngine::drawLine(const Vector3& start, const Vector3& end){
  bool found = false;

  FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount && !found){
    LineRenderer* lineRenderer = mLineRenderers->get(i);

    if(! lineRenderer->mActive){
      found = true;
      lineRenderer->init();
      lineRenderer->set(start, end);
      lineRenderer->mActive = true;
    }
  }
}

// ---------------------------------------------------------------------------

} /* namespace DE */
