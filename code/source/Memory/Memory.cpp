#include "Memory.hpp"
#include "FreeListAllocator.hpp"
#include "LinearAllocator.hpp"
#include "Log.hpp"

namespace DE {

// FreeListAllocator Memory::smGlobal;
LinearAllocator Memory::smGlobal;

// ---------------------------------------------------------------------------

Memory::Memory() {

}

// ---------------------------------------------------------------------------

Memory::~Memory() {

}

// ---------------------------------------------------------------------------

void Memory::init() {

	// NOTE: Log starts here!!
	Log::init();

	TRACE();

	smGlobal.init(2 * GB);

	ECHO("TOTAL MEMORY (IN MB)");VAL(f32,(smGlobal.getTotalSize() / MB));
}

// ---------------------------------------------------------------------------

void Memory::init(u32 size) {
	TRACE();

	smGlobal.init(size);
}

// ---------------------------------------------------------------------------

void Memory::flush() {
	smGlobal.flush();
}

// ---------------------------------------------------------------------------

Allocator& Memory::getGlobal() {
	return smGlobal;
}

// ---------------------------------------------------------------------------

void Memory::free() {

	ECHO("MEMORY SIZE (IN MB)");
	VAL(f32,(smGlobal.getTotalSize() / MB));
	ECHO("REMAINING ALLOCATED MEMORY (IN MB)");
	VAL(f32,(smGlobal.getAllocatedSize() / MB));

	smGlobal.terminate();

	// NOTE: Log ends here!!
	Log::terminate();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
