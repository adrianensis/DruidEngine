#include "Memory.h"
#include "FreeListAllocator.h"

namespace DE {

FreeListAllocator Memory::smGlobal;

// ---------------------------------------------------------------------------

Memory::Memory(){

}

// ---------------------------------------------------------------------------

Memory::~Memory(){

}

// ---------------------------------------------------------------------------

void Memory::init(){
	TRACE();

  u32 GB = 1024*1024*1024;
  smGlobal.init(1*GB);
}

// ---------------------------------------------------------------------------

void Memory::init(u32 size){
	TRACE();

  smGlobal.init(size);
}

// ---------------------------------------------------------------------------

Allocator& Memory::getGlobal(){
  return smGlobal;
}

// ---------------------------------------------------------------------------

void Memory::free(){

	ECHO("REMAINING ALLOCATED MEMORY (IN MB)");
	VAL(f32,(smGlobal.getAllocatedSize() / (1024.0f*1024.0)));

  smGlobal.reset();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
