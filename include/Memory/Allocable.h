#ifndef ALLOCABLE_H_
#define ALLOCABLE_H_

#include "Allocator.h"

namespace DE {

/*!
    \brief Object that can be allocated by an allocator.
*/
class Allocable {

public:

    Allocator* mAllocator;

    /*!
        \brief Default Constructor.
    */
    Allocable(){};

    /*!
        \brief Destructor.
    */
    virtual ~Allocable(){};

    /*!
        \brief Constructor. Sets the allocator.
        \param allocator Pointer to Allocator.
    */
    void init(Allocator* allocator){ mAllocator = allocator; };

    /*!
        \brief Copy constructor.
        \param other Other Allocable.
    */
    void init(const Allocable& other){ mAllocator = other.mAllocator; };

};

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \related Allocable
    \tparam T Class. Must be Allocable.
    \param allocator Allocator used to allocate memory.
    \param alignment Bytes alignment.
*/
template<class T>
T* allocate(Allocator& allocator, u32 alignment){
    Allocable* obj = static_cast<Allocable*>(allocator.allocate(sizeof(T), alignment));
    obj->init(&allocator);
    return static_cast<T*>(obj);
};

/*!
    \brief Constructs objects. It is used like "new" keyword.
    Allocate an object of T class, using the allocator.

    \related Allocable
    \tparam T Class
    \param allocator Allocator used to allocate memory.
*/
template<class T>
T* allocate(Allocator& allocator){
    Allocable* obj = static_cast<Allocable*>(allocator.allocate(sizeof(T)));
    obj->init(&allocator);
    return static_cast<T*>(obj);
};

} /* namespace DE */

#endif /* ALLOCABLE_H_ */
