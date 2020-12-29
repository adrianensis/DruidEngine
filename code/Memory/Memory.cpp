#include "Memory.hpp"
#include "FreeListAllocator.hpp"
#include "LinearAllocator.hpp"
#include "Log.hpp"

namespace DE {

std::map<std::string, u32> Memory::memoryMapCounter;

FreeListAllocator Memory::smGlobal;
//LinearAllocator Memory::smGlobal;

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

	DE_TRACE()

	smGlobal.init(2 * GB);

	ECHO("TOTAL MEMORY (IN MB)");
	VAL(f32,(smGlobal.getTotalSize() / MB));
}

// ---------------------------------------------------------------------------

void Memory::init(u32 size) {
	DE_TRACE()

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

	ECHO("MAX ALLOCATED MEMORY(IN MB)");
	VAL(f32,(smGlobal.getDebugMaxAllocatedSize() / MB));

	ECHO("REMAINING ALLOCATED MEMORY (IN MB)");
	VAL(f32,(smGlobal.getAllocatedSize() / MB));

	ECHO("REMAINING ALLOCATED MEMORY (BYTES)");
	VAL(f32,(smGlobal.getAllocatedSize()));

	smGlobal.terminate();

    std::map<std::string, u32>::iterator it = memoryMapCounter.begin();
    while(it != memoryMapCounter.end())
    {
        std::cout<< (it->second == 0 ? "OK" : "!!") << " " << it->second<<" :: "<<it->first<<std::endl;
        it++;
    }

	// NOTE: Log ends here!!
	Log::terminate();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
