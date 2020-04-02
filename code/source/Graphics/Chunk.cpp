#include "Chunk.h"

#include "Batch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"
#include "HashMap.h"
#include "MathUtils.h"
#include "GameObject.h"
#include "Transform.h"

namespace DE {

// ---------------------------------------------------------------------------

BatchesMap::BatchesMap() : DE_Class() {
  mBatches = nullptr;
}

BatchesMap::~BatchesMap() {
  FOR_LIST(it, mBatches->getValues()){
    Memory::free<Batch>(it.get());
  }

  Memory::free<HashMap<Texture*, Batch*>>(mBatches);
}

// ---------------------------------------------------------------------------

void BatchesMap::init() {
  mBatches = Memory::allocate<HashMap<Texture*, Batch*>>();
  mBatches->init();
}

// ---------------------------------------------------------------------------

void BatchesMap::addRenderer(Renderer* renderer) {

  Texture* texture = renderer->getMaterial()->getTexture();

  if( ! mBatches->contains(texture)) {

    Batch* batch = Memory::allocate<Batch>();
    batch->init(renderer->getMesh(), renderer->getMaterial());

    mBatches->set(texture, batch);
  }

  mBatches->get(texture)->addRenderer(renderer);
}

// ---------------------------------------------------------------------------

void BatchesMap::bind() {
	FOR_LIST(it, mBatches->getValues()){
		it.get()->bind();
	}
}

// ---------------------------------------------------------------------------

u32 BatchesMap::render(u32 layer) {
  u32 drawCallCounter = 0;

  FOR_LIST(it, mBatches->getValues()){
		drawCallCounter += it.get()->render(layer);
	}

  return drawCallCounter;
}

// ---------------------------------------------------------------------------

Chunk::Chunk() : DE_Class() {
  mLeftTop = Vector2(0,0);
  mRadius = 0;
  mSize = 0;
  mIsLoaded = false;

  mBatchesMap = Memory::allocate<BatchesMap>();
}

Chunk::~Chunk() {
  Memory::free<BatchesMap>(mBatchesMap);
}

// ---------------------------------------------------------------------------

void Chunk::init(){

  mBatchesMap = Memory::allocate<BatchesMap>();
  mBatchesMap->init();

  mLeftTop.set(0,0,0);
}

// ---------------------------------------------------------------------------

void Chunk::set(const Vector3& leftTop, f32 size){
  mLeftTop = leftTop;
  mSize = size;
  f32 halfSize = mSize / 2.0f;
  mCenter.set(mLeftTop.x + halfSize, mLeftTop.y - halfSize, 0);

  mRadius = mCenter.dst(mLeftTop);
}

// ---------------------------------------------------------------------------

void Chunk::load(){
    mIsLoaded = true;
}

// ---------------------------------------------------------------------------

void Chunk::unload(){
  if(mIsLoaded){
    mIsLoaded = false;
  }
}

// ---------------------------------------------------------------------------

bool Chunk::isLoaded() {
  return mIsLoaded;
}

// ---------------------------------------------------------------------------

void Chunk::addRenderer(Renderer* renderer) {
  mBatchesMap->addRenderer(renderer);
}

// ---------------------------------------------------------------------------

void Chunk::bind() {
  mBatchesMap->bind();
}

// ---------------------------------------------------------------------------

u32 Chunk::render(u32 layer) {
  return mBatchesMap->render(layer);
}

// ---------------------------------------------------------------------------

bool Chunk::containsRenderer(const Renderer* renderer){
  Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
  return MathUtils::testRectanglePoint(mLeftTop, mSize, mSize, rendererPosition, 0);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
