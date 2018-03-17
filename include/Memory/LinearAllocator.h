#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"
#include "Basic.h"

namespace DE {

/*!
    \brief Allocates memory in a linear way. The whole memory is freed in one shot.
*/
DE_CLASS(LinearAllocator) DE_CLASS_EXTENDS public Allocator {

public:

    /*!
        \brief Default Constructor.
    */
    LinearAllocator();

    /*!
        \brief Destructor.
    */
    virtual ~LinearAllocator();

    virtual void init(const u32 size);
    virtual void* allocate(const u32 size);
    virtual void* allocate(const u32 size, const u32 alignment);
    virtual void free(const void* pointer);
    virtual void reset();

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
