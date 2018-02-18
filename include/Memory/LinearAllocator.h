#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

/*!
    \brief Allocates memory in a linear way. The whole memory is freed in one shot.
*/
class LinearAllocator : public Allocator {

public:

    /*!
        \brief Default Constructor.
    */
    LinearAllocator();

    /*!
        \brief Destructor.
    */
    virtual ~LinearAllocator();

    virtual void init(u32 size);
    virtual void* allocate(u32 size);
    virtual void* allocate(u32 size, u32 alignment);
    virtual void free(const void* pointer);
    virtual void reset();

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
