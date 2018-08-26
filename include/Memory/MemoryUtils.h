#ifndef DE_MEMORYUTILS_H
#define DE_MEMORYUTILS_H

#include "Allocator.h"
#include <typeinfo>
#include <type_traits>

/*!
   \file MemoryUtils.h
   \brief Static functions for memory allocation.
*/

namespace DE {

/*!
  \brief Constructs objects. It is used like "new" keyword.
  Allocate an object of T class, using the allocator.

  \tparam T Class.
  \param allocator Allocator used to allocate memory.
  \param alignment Bytes alignment.
*/
template<class T>
static T* allocate(Allocator& allocator, const u32 alignment){
  T* object = new(allocator.allocate(sizeof(T), alignment)) T;
  return object;
};

/*!
  \brief Constructs objects. It is used like "new" keyword.
  Allocate an object of T class, using the allocator.

  \tparam T Class
  \param allocator Allocator used to allocate memory.
*/
template<class T>
static T* allocate(Allocator& allocator){
  T* object = new(allocator.allocate(sizeof(T))) T;
  return object;
};

/*!
  \brief Destroys objects. It is used like "delete" keyword.
  Deallocate data, using the allocator.

  \tparam pointer pointer to data
  \param allocator Allocator used to deallocate memory.
*/
template<class T>
static void free(T* object, Allocator& allocator){
  if(object != nullptr){
    object->~T();
    allocator.free(object);
  }
};

} /* namespace DE */

#endif /* DE_MEMORYUTILS_H */
