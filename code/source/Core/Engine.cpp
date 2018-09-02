#include "Engine.h"
#include "Memory.h"
#include "RenderContext.h"

namespace DE {

Engine::Engine() : DE_Class(){

};

Engine::~Engine() = default;

void Engine::init(){
  Memory::init();
  mRender.init();
};

void Engine::run(){
	while(! RenderContext::isClosed()){
		mRender.step();
	}

	mRender.terminate();
};

void Engine::terminate() {
	Memory::free();
}

} /* namespace DE */
