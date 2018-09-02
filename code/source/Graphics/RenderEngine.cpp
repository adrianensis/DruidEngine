#include "RenderEngine.h"

#include "Memory.h"

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

	Mesh* mesh = Memory::allocate<Mesh>();
	mesh->init(3);

	mesh->addVertex(Vector3(-0.5f, -0.5f, 0.0f))->
			addVertex(Vector3(0.5f, -0.5f, 0.0f))->
			addVertex(Vector3(0.0f, 0.5f, 0.0f))->
			addFace(0,1,2)->
			close();

	Batch* batch = Memory::allocate<Batch>();
	batch->init(mesh, nullptr);
	batch->bind();
	mTextureBatches->pushBack(batch);
}


void RenderEngine::step() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Clear the color and the depth buffer

	auto it = mTextureBatches->getIterator();

	for (; !it.isNull(); it.next())
		it.get()->render();

	RenderContext::swap();
}

void RenderEngine::terminate() {

	Memory::free<List<Batch*>>(mTextureBatches);
	Memory::free<RenderContext>(mRenderContext);
	Memory::free<Camera>(mCamera);

	RenderContext::terminate();
}

} /* namespace DE */
