#ifndef MEMORYUTILS_H_
#define MEMORYUTILS_H_

#include "IAllocator.h"
#include <typeinfo>
#include <type_traits>

/*!
   \file MemoryUtils.h
   \brief Functions for memory allocation.
*/

namespace DE {

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \tparam T Class.
    \param allocator IAllocator used to allocate memory.
    \param alignment Bytes alignment.
*/
template<class T>
static T* allocate(IAllocator& allocator, const u32 alignment){
    T* obj = new(allocator.allocate(sizeof(T), alignment)) T;
    return obj;
};

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \tparam T Class
    \param allocator IAllocator used to allocate memory.
*/
template<class T>
static T* allocate(IAllocator& allocator){
    T* obj = new(allocator.allocate(sizeof(T))) T;
    return obj;
};

} /* namespace DE */

#endif /* MEMORYUTILS_H_ */
