#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"
#include "BasicTypes.h"

namespace DE {

/*!
    \brief Allocates memory in FIFO strategy.
*/
class StackAllocator : public LinearAllocator {

private:

    void* mTop;

    static u32 smHeaderSize;
    void storeHeader(const void* address, u32 size);

public:

    /*!
        \brief Default Constructor.
    */
    StackAllocator();

    /*!
        \brief Destructor.
    */
    virtual ~StackAllocator();

    /*!
        \return Pointer to the top of the stack.
    */
    void* getTop();

    virtual void init(u32 size);
    virtual void* allocate(u32 size);
    virtual void* allocate(u32 size, u32 alignment);
    virtual void free(const void* pointer);
    virtual void freeAligned(const void* pointer);

    /*!
        \brief Free the top of the stack.
    */
    void free();

    /*!
        \brief Free the (aligned) top of the stack.
    */
    void freeAligned();

    virtual void reset();

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
