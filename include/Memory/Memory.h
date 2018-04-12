#ifndef MEMORY_H_
#define MEMORY_H_

#include "FreeListAllocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "MemoryUtils.h"
#include <string>

namespace DE {

class Memory : public DE_Class {

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

    static void init(const u32 size);

    static IAllocator& getGlobal();

    template<class T>
    static T* allocate(u32 alignment){
        return DE::allocate<T>(smGlobal, alignment);
    }

    template<class T>
    static T* allocate(){
        return DE::allocate<T>(smGlobal);
    }

    static void free();

};

} /* namespace DE */

#endif /* MEMORY_H_ */
