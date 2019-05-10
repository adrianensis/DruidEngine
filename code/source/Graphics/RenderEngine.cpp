#include "RenderEngine.h"
#include "Memory.h"
#include "Batch.h"
#include "RenderContext.h"
#include "Camera.h"

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
	mesh->init(4);

	mesh->open()->
		addVertex(Vector3(-0.5f, 0.5f, 0.0f))->
		addVertex(Vector3(-0.5f, -0.5f, 0.0f))->
		addVertex(Vector3(0.5f, -0.5f, 0.0f))->
		addVertex(Vector3(0.5f, 0.5f, 0.0f))->
		addFace(0,1,3)->
		addFace(1,2,3)->
		close();

	Batch* batch = Memory::allocate<Batch>();
	batch->init(mesh, nullptr);
	mTextureBatches->pushBack(batch);
}


void RenderEngine::step() {

	auto it = mTextureBatches->getIterator();

	for (; !it.isNull(); it.next()){
		it.get()->render();
	}

	RenderContext::swap();
}

void RenderEngine::bind() {

	auto it = mTextureBatches->getIterator();

	for (; !it.isNull(); it.next()){
		it.get()->bind();
	}
}

void RenderEngine::update() {

	auto it = mTextureBatches->getIterator();

	for (; !it.isNull(); it.next()){
		it.get()->update();
	}
}

void RenderEngine::terminate() {

	Memory::free<List<Batch*>>(mTextureBatches); // TODO : foreach, clean batches ?
	Memory::free<RenderContext>(mRenderContext);
	Memory::free<Camera>(mCamera);

	RenderContext::terminate();
}

} /* namespace DE */
