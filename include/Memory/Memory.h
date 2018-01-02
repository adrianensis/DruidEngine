#ifndef MEMORY_H_
#define MEMORY_H_

#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"
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

    static LinearAllocator mGlobal;

    static DE::Array<bool>* mFlags;
    static DE::Array<PoolAllocator>* mPools;

    //std::map <string, PoolAllocator> mPools;

public:

    Memory(){};
    ~Memory(){ mGlobal.reset(); };

    // static void init(){
    //     mGlobal.init(1024*1024); // TODO: review
    //
    //     mPools = DE::allocate<Array<PoolAllocator>>(mGlobal);
    //     mPools->init(5);
    //
    //     mPools[0].init(sizeof(Matrix4),10);
    //     mPools[1].init(sizeof(Vector2),10);
    //     mPools[2].init(sizeof(Vector3),10);
    //     mPools[3].init(sizeof(Vector4),10);
    //     mPools[4].init(sizeof(Quaternion),10);
    // };
    //
    // template<class T>
    // static T* allocate(){
    //     return mPools[getIndex<T>()].allocateBlock();
    // }
    //
    // static void allocate(){
    //     return mPools[getIndex<T>()].allocateBlock();
    // }

};

} /* namespace DE */

#endif /* MEMORY_H_ */
