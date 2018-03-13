#include "Memory.h"
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

FreeListAllocator Memory::smGlobal;

Memory::Memory(){

}

Memory::~Memory(){

}

void Memory::init(){
    u32 GB = 1024*1024*1024;
    smGlobal.init(1*GB);
};

void Memory::init(u32 size){
    smGlobal.init(size);
};

Allocator& Memory::getGlobal(){
    return smGlobal;
}

void Memory::free(){
    smGlobal.reset();
}

} /* namespace DE */
