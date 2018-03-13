#ifndef MEMORY_H_
#define MEMORY_H_

#include "FreeListAllocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "MemoryUtils.h"
#include <type_traits>
#include <string>

namespace DE {

class Memory {

private:

    /*
        * Global Heap for general purposes
        * Special Heap for gameObjects (spawn and destroy)
        * Level-Loading Heap
        * Stack-allocator for single-frame allocations (cleared every frame)
        * Debug memory Heap, only for allocations that will not be needed in final game.
    */

    static FreeListAllocator smGlobal;

    // static DE::Array<bool>* mFlags;
    // static DE::Array<PoolAllocator>* mPools;

    //std::map <string, PoolAllocator> mPools;

    Memory();
    ~Memory();

public:

    // static FreeListAllocator smGlobal;

    static void init();

    static void init(u32 size);

    static Allocator& getGlobal();

    template<class T>
    static T* allocate(u32 alignment);

    template<class T>
    static T* allocate();

    static void free();

};

} /* namespace DE */

#endif /* MEMORY_H_ */
