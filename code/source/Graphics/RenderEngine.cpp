#include "RenderEngine.h"
#include "Memory.h"
#include "Batch.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Frustum.h"
#include "Renderer.h"
#include "List.h"
#include "Array.h"
#include "HashMap.h"
#include "Log.h"
#include "Shader.h"
#include "Vector3.h"
#include "MathUtils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Settings.h"
#include "Chunk.h"
#include "UI.h"

namespace DE {

// ---------------------------------------------------------------------------

RenderEngine::LineRenderer::LineRenderer() : DE_Class(){
  mVertices = nullptr;
  mActive = false;
}

RenderEngine::LineRenderer::~LineRenderer(){
  Memory::free<Array<f32>>(mVertices);
}

// ---------------------------------------------------------------------------

void RenderEngine::LineRenderer::init(){
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

void RenderEngine::LineRenderer::bind(const Array<u32>* indices){
  mVAO = RenderContext::createVAO();
  mVBOPosition = RenderContext::createVBO(mVertices, 3, 0);
  mEBO = RenderContext::createEBO(indices);

  RenderContext::enableVAO(mVAO);
}

// ---------------------------------------------------------------------------

RenderEngine::RenderEngine() : DE_Class(), Singleton(){
	mCamera = nullptr;
  mLineRenderers = nullptr;
  mLineRendererIndices = nullptr;
  mShaderLine = nullptr;
}

RenderEngine::~RenderEngine() = default;

// ---------------------------------------------------------------------------

void RenderEngine::init(f32 sceneSize){
	TRACE();

  mLineRenderers = Memory::allocate<Array<LineRenderer*>>();
  mLineRendererIndices = Memory::allocate<Array<u32>>();
  mShaderLine = Memory::allocate<Shader>();


  // Line Renderers

  mLineRenderersCount = Settings::getInstance()->getF32("line.renderers.count");
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

  // Static Chunks grid

  f32 chunksGridSize = Settings::getInstance()->getF32("scene.chunks.count");
  f32 chunksGridSizeHalf = chunksGridSize/2.0f;

  mStaticChunks = Memory::allocate<Array<Chunk*>>();
  mStaticChunks->init(chunksGridSize*chunksGridSize); // TODO : define how many chunks to create. Move to Settings.

  f32 chunkSize = sceneSize / ((f32) chunksGridSize);

  u32 count = 0;
  for(i32 i = -chunksGridSizeHalf; i < chunksGridSizeHalf; ++i){
    for(i32 j = chunksGridSizeHalf; j > -chunksGridSizeHalf; --j){
      Chunk* chunk = Memory::allocate<Chunk>();
      chunk->init();
      chunk->set(Vector2(i*chunkSize, j*chunkSize), chunkSize);

      mStaticChunks->set(count, chunk);
      count++;
    }
  }

  // Dynamic Chunk

  mDynamicChunk = Memory::allocate<Chunk>();
  mDynamicChunk->init();
  mDynamicChunk->set(Vector2(-chunksGridSizeHalf*chunkSize, chunksGridSizeHalf*chunkSize), sceneSize); // Size = The WHOLE scene

  mMaxLayersCount = 10;
  mMaxLayersUsed = 0;
}

// ---------------------------------------------------------------------------

void RenderEngine::step(){

  mCamera->getFrustum()->build();
  mCamera->calculateInverseMatrix();

  //check Chunks
  FOR_ARRAY(i, mStaticChunks){
    Chunk* chunk = mStaticChunks->get(i);

    bool chunkInCameraView = mCamera->getFrustum()->testSphere(chunk->mCenter, chunk->mRadius*2);

    if(chunkInCameraView){
      chunk->load();
    } else {
      chunk->unload();
    }
  }

  u32 drawCallCounter = 0;

  FOR_RANGE(layer, 0, mMaxLayersUsed){

    FOR_ARRAY(i, mStaticChunks){
      Chunk* chunk = mStaticChunks->get(i);

      if(chunk->isLoaded()){
        drawCallCounter += chunk->render(layer);
      }
    }

    // NOTE : It only loads once.
    if(!mDynamicChunk->isLoaded()){
      mDynamicChunk->load();
    }

    drawCallCounter += mDynamicChunk->render(layer);

	}

  // VAR(u32,drawCallCounter);

  stepDebug();

	RenderContext::swap();

}// ---------------------------------------------------------------------------

void RenderEngine::stepDebug(){

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

      glLineWidth(1);
      glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

      RenderContext::enableVAO(0);

      lineRenderer->mActive = false;
    }
  }
}

// ---------------------------------------------------------------------------

void RenderEngine::bind(){
  // FOR_ARRAY(i, mStaticChunks){
  //   mStaticChunks->get(i)->bind();
  // }
  //
  // mDynamicChunk->bind();
}

// ---------------------------------------------------------------------------

void RenderEngine::terminate(){
  TRACE();

  Memory::free<Array<u32>>(mLineRendererIndices);
  Memory::free<Shader>(mShaderLine);

  FOR_ARRAY_COND(i, mLineRenderers, i < mLineRenderersCount){
    Memory::free<LineRenderer>(mLineRenderers->get(i));
  }

  Memory::free<Array<LineRenderer*>>(mLineRenderers);

  FOR_ARRAY(i, mStaticChunks){
    Memory::free<Chunk>(mStaticChunks->get(i));
  }

  Memory::free<Array<Chunk*>>(mStaticChunks);

  Memory::free<Chunk>(mDynamicChunk);
  Memory::free<UI>(UI::getInstance());
}

// ---------------------------------------------------------------------------

void RenderEngine::addRenderer(Renderer* renderer){

  if(renderer->isStatic()){
    // Create batches
  	Texture* texture = renderer->getMaterial()->getTexture();

    bool found = false;
    FOR_ARRAY_COND(i, mStaticChunks, !found){
      Chunk* chunk = mStaticChunks->get(i);
      if((! renderer->getChunk()) && chunk->containsRenderer(renderer)){
        chunk->addRenderer(renderer);
        renderer->setChunk(chunk);
        found = true;
      }
    }
  }else{
    mDynamicChunk->addRenderer(renderer);
    renderer->setChunk(mDynamicChunk);
  }

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
