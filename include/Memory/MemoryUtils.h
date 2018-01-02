#ifndef MEMORYUTILS_H_
#define MEMORYUTILS_H_

#include "Allocator.h"
#include "Debug.h"
#include "Container.h"
#include <typeinfo>
#include <type_traits>

/*!
   \file MemoryUtils.h
   \brief Functions for memory allocation.
*/

namespace DE {

template<class T>
static void checkContainer(T* pointer, Allocator& allocator){
    if(std::is_base_of<Container,T>::value){
        Container* a = dynamic_cast<Container*>(pointer);
        a->setAllocator(&allocator);
    }
    else{
        // ECHO("Object is not Allocable.");
    }
}

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \tparam T Class.
    \param allocator Allocator used to allocate memory.
    \param alignment Bytes alignment.
*/
template<class T>
static T* allocate(Allocator& allocator, u32 alignment){
    T* obj = static_cast<T*>(allocator.allocate(sizeof(T), alignment));
    checkContainer<T>(obj, allocator);
    return obj;
};

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \tparam T Class
    \param allocator Allocator used to allocate memory.
*/
template<class T>
static T* allocate(Allocator& allocator){
    T* obj = static_cast<T*>(allocator.allocate(sizeof(T)));
    checkContainer<T>(obj, allocator);
    return obj;
};

} /* namespace DE */

#endif /* MEMORYUTILS_H_ */
