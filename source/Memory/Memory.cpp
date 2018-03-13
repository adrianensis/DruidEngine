#include "Memory.h"
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

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

template<class T>
T* Memory::allocate(u32 alignment){
    return DE::allocate<T>(smGlobal, alignment);
}

template<class T>
T* Memory::allocate(){
    return DE::allocate<T>(smGlobal);
}

void Memory::free(){
    smGlobal.reset();
}

} /* namespace DE */
