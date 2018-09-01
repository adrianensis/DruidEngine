#include "RenderEngine.h"

#include "Memory.h"

namespace DE {

RenderEngine::RenderEngine() :
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
}


void RenderEngine::step() {

	auto it = mTextureBatches->getIterator();

	for (; !it.isNull(); it.next())  ;
		//draw

	RenderContext::swap();
}

void RenderEngine::terminate() {

	Memory::free<List<Batch*>>(mTextureBatches);
	Memory::free<RenderContext>(mRenderContext);
	Memory::free<Camera>(mCamera);

	RenderContext::terminate();
}

} /* namespace DE */
