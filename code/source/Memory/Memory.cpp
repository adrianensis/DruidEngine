#include "Memory.h"
#include "FreeListAllocator.h"
#include "LinearAllocator.h"
#include "Log.h"

namespace DE {

// FreeListAllocator Memory::smGlobal;
LinearAllocator Memory::smGlobal;

// ---------------------------------------------------------------------------

Memory::Memory(){

}

// ---------------------------------------------------------------------------

Memory::~Memory(){

}

// ---------------------------------------------------------------------------

void Memory::init(){

	// NOTE: Log stats here!!
	Log::init();

	TRACE();

  smGlobal.init(2*GB);

	ECHO("TOTAL MEMORY (IN MB)");
	VAL(f32,(smGlobal.getTotalSize() / MB));
}

// ---------------------------------------------------------------------------

void Memory::init(u32 size){
	TRACE();

  smGlobal.init(size);
}

// ---------------------------------------------------------------------------

void Memory::flush(){
	smGlobal.flush();
}

// ---------------------------------------------------------------------------

Allocator& Memory::getGlobal(){
  return smGlobal;
}

// ---------------------------------------------------------------------------

void Memory::free(){

	ECHO("REMAINING ALLOCATED MEMORY (IN MB)");
	VAL(f32,(smGlobal.getAllocatedSize() / MB));

  smGlobal.terminate();

	// NOTE: Log ends here!!
	Log::terminate();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
