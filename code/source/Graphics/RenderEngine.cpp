#include "RenderEngine.h"
#include "Memory.h"
#include "Batch.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Renderer.h"
#include "List.h"

namespace DE {

RenderEngine::RenderEngine() : DE_Class(),
	mTextureBatches(nullptr),
	mRenderContext(nullptr),
	mCamera(nullptr)
{
}

RenderEngine::~RenderEngine() = default;

void RenderEngine::init() {

	mTextureBatches = Memory::allocate<List<Batch*>>();
	mRenderContext = Memory::allocate<RenderContext>();
	mCamera = Memory::allocate<Camera>();

	RenderContext::init();
	mTextureBatches->init();

	Batch* batch = Memory::allocate<Batch>();

	// TODO : don't initialize batch here
	batch->init(nullptr, nullptr);

	mTextureBatches->pushBack(batch);
}


void RenderEngine::step() {

	u32 i=0;
	for (auto it = mTextureBatches->getIterator(); !it.isNull(); it.next()){
		ECHO("BATCH NUM")
		VAL(u32,i+1)
		it.get()->render();
	}

	RenderContext::swap();
}

void RenderEngine::bind() {

	for (auto it = mTextureBatches->getIterator(); !it.isNull(); it.next()){
		it.get()->bind();
	}
}

void RenderEngine::update() {

	for (auto it = mTextureBatches->getIterator(); !it.isNull(); it.next()){
		it.get()->update();
	}
}

void RenderEngine::terminate() {

	Memory::free<List<Batch*>>(mTextureBatches); // TODO : foreach, clean batches ?
	Memory::free<RenderContext>(mRenderContext);
	Memory::free<Camera>(mCamera);

	RenderContext::terminate();
}

void RenderEngine::addRenderer(Renderer* renderer) {
	// TODO :initialize batch here
	// if batch doesn't exist, create.

	mTextureBatches->get(0)->setMesh(renderer->getMesh());
	mTextureBatches->get(0)->addRenderer(renderer);
}

} /* namespace DE */
