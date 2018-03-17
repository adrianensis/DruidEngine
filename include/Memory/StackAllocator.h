#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"
#include "Basic.h"

namespace DE {

/*!
    \brief Allocates memory in FIFO strategy.
*/
DE_CLASS(StackAllocator) DE_CLASS_EXTENDS public LinearAllocator {

private:

    void* mTop;

    static u32 smHeaderSize;
    void storeHeader(const void* address, const u32 size);

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

    virtual void init(const u32 size);
    virtual void initFromMemory(const u32 size, void* mem);
    virtual void* allocate(const u32 size);
    virtual void* allocate(const u32 size, const u32 alignment);
    virtual void free(const void* pointer);

    /*!
        \brief Free the top of the stack.
    */
    void free();

    virtual void reset();

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
