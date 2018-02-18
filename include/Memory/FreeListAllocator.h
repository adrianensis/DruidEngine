#ifndef FREELISTALLOCATOR_H_
#define FREELISTALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"
#include "List.h"
#include "LinearAllocator.h"

namespace DE {

/*!
    \brief List of memory blocks.
*/
class FreeListAllocator : public Allocator {

private:

    class Block{
    public:
        void* unalignedAddress;
        u32 size;

        Block();
        ~Block();
        void init(void* unalignedAddress, u32 size);

        Block& operator=(const Block& rhs) {
            if (this == &rhs) return *this; // handle self assignment
            this->unalignedAddress = rhs.unalignedAddress;
            this->size = rhs.size;
            return (*this);
        }
    };

    LinearAllocator mLinearAllocator; // TODO: change for Memory::allocate()
    List<Block>* mFreeBlocks; // TODO: change to Block*
    List<Block>* mUsedBlocks;

    Block allocateBlock(u32 size);
    u32 freeBlock(void* unalignedAddress);

public:

    /*!
        \brief Default Constructor.
    */
    FreeListAllocator();

    /*!
        \brief Destructor.
    */
    virtual ~FreeListAllocator();

    virtual void init(u32 size);
    virtual void* allocate(u32 size);
    virtual void* allocate(u32 size, u32 alignment);
    virtual void free(const void* pointer);
    virtual void reset();

};

} /* namespace DE */

#endif /* FREELISTALLOCATOR_H_ */
