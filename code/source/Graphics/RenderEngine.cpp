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
#include "Debug.h"
#include "Shader.h"
#include "Vector3.h"

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

RenderEngine::~RenderEngine(){
  Memory::free<HashMap<Texture*, Batch*>>(mBatches);
  Memory::free<Array<LineRenderer*>>(mLineRenderers);
  Memory::free<Array<u32>>(mLineRendererIndices);
  Memory::free<Shader>(mShaderLine);

  for (u32 i = 0; i < mLineRenderersCount; ++i){
    Memory::free<LineRenderer>(mLineRenderers->get(i));
  }
}

// ---------------------------------------------------------------------------

void RenderEngine::init() {

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

  for (u32 i = 0; i < mLineRenderersCount; ++i){
    LineRenderer* lineRenderer = Memory::allocate<LineRenderer>();
    lineRenderer->init();
    lineRenderer->mActive = false;

    mLineRenderers->set(i,lineRenderer);
  }
}

// ---------------------------------------------------------------------------

void RenderEngine::step() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		it.get()->render();
	}

  stepDebug();

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

  for (u32 i = 0; i < mLineRenderersCount; ++i){
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

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		it.get()->bind();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::update() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		it.get()->update();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::terminate() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		Memory::free<Batch>(it.get());
	}

	Memory::free<HashMap<Texture*, Batch*>>(mBatches);

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
}
// ---------------------------------------------------------------------------

void RenderEngine::drawLine(const Vector3& start, const Vector3& end){
  bool found = false;

  for (u32 i = 0; i < mLineRenderersCount && !found; ++i) {
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

void RenderEngine::setCamera(Camera* camera){
	mCamera = camera;
}

// ---------------------------------------------------------------------------

Camera* RenderEngine::getCamera() const {
 	return mCamera;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
