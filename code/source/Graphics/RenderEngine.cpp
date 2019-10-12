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
}

RenderEngine::LineRenderer::~LineRenderer() {
  Memory::free<Array<f32>>(mVertices);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::init(const Vector3& start, const Vector3& end, const Array<u32>* indices) {
  mVertices = Memory::allocate<Array<f32>>();

  mVertices->init(2*3); // 1 vertex = 3 floats

  mVertices->set(0,start.x);
  mVertices->set(1,start.y);
  mVertices->set(2,start.z);
  mVertices->set(3,end.x);
  mVertices->set(4,end.y);
  mVertices->set(5,end.z);

  mVAO = RenderContext::createVAO();
  mVBOPosition = (u32) RenderContext::createVBO(mVertices, 3, 0);
  mEBO = (u32) RenderContext::createEBO(indices);
  RenderContext::enableVAO(0);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::bind() {
  RenderContext::enableVAO(mVAO);
}

// ---------------------------------------------------------------------------

RenderEngine::RenderEngine() : DE_Class(), Singleton() {
	mBatches = nullptr;
	mCamera = nullptr;
  mLineRenderers = nullptr;
  mLineRendererIndices = nullptr;
  mShaderLine = nullptr;
}

RenderEngine::~RenderEngine(){
  Memory::free<HashMap<Texture*, Batch*>>(mBatches);
  Memory::free<List<LineRenderer*>>(mLineRenderers);
  Memory::free<Array<u32>>(mLineRendererIndices);
  Memory::free<Shader>(mShaderLine);
}

// ---------------------------------------------------------------------------

void RenderEngine::init() {

	mBatches = Memory::allocate<HashMap<Texture*, Batch*>>();
  mLineRenderers = Memory::allocate<List<LineRenderer*>>();
  mLineRendererIndices = Memory::allocate<Array<u32>>();
  mShaderLine = Memory::allocate<Shader>();

  RenderContext::init();
  mBatches->init();

  mLineRenderers->init();

  mLineRendererIndices->init(2);
  mLineRendererIndices->set(0,0.0f);
  mLineRendererIndices->set(1,1.0f);

  mShaderLine->initDebug();
}

// ---------------------------------------------------------------------------

void RenderEngine::step() {

  stepDebug();

	u32 i=0;
	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		//ECHO("BATCH NUM")
		//VAL(u32,i+1)
		it.get()->render();
	}

	RenderContext::swap();

}// ---------------------------------------------------------------------------

void RenderEngine::stepDebug() {

  mShaderLine->use();

  const Matrix4& projectionMatrix = getCamera()->getProjectionMatrix();
  const Matrix4& viewTranslationMatrix = getCamera()->getViewTranslationMatrix();
  const Matrix4& viewRotationMatrix = getCamera()->getViewRotationMatrix();

  u32 i=0;
	for (auto it = mLineRenderers->getIterator(); !it.isNull(); it.next()){

    it.get()->bind();

    mShaderLine->addMatrix(projectionMatrix, "projectionMatrix");
    mShaderLine->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
    mShaderLine->addMatrix(viewRotationMatrix, "viewRotationMatrix");

    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

    RenderContext::enableVAO(0);
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
  LineRenderer* lineRenderer = Memory::allocate<LineRenderer>();
  lineRenderer->init(start, end, mLineRendererIndices);

  mLineRenderers->pushBack(lineRenderer);
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
