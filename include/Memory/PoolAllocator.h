#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include "LinearAllocator.h"
#include "Basic.h"

namespace DE {

/*!
    \brief Allocates memory using fixed-size blocks. The blocks can be freed.
*/
DE_CLASS(PoolAllocator) DE_CLASS_EXTENDS public LinearAllocator {

private:

    u32 mBlockSize;
    u32 mFullBlockSize;
    u32 mAlignment;
    u32 mMaxBlocks;
    u32 mUsedBlocks;
    void* mFirst;
    void* mLast;

    static u32 smPtrSize;

    static void storePointer(void* address, const void* pointer);
    static void* getNextIterator(void* it);

    void* getBlock(const void* it);
    void* getIteratorFromBlock(const void* block);

    void checkAllocateBlock();
    void checkFreeBlock();

    void _init(u32 blockSize, u32 numBlocks, void* mem, u32 alignment);

public:

    /*!
        \brief Default Constructor.
    */
    PoolAllocator();

    /*!
        \brief Destructor.
    */
    virtual ~PoolAllocator();

    /*!
        \return The count of free blocks.
    */
    u32 getFreeBlocks();

    /*!
        \brief Constructor.
        \param blockSize Size of a single block.
        \param numBlocks Number of blocks.
        \param alignment Bytes alignment.
    */
    void init(u32 blockSize, u32 numBlocks, u32 alignment);

    /*!
        \brief Constructor.
        \param blockSize Size of a single block.
        \param numBlocks Number of blocks.
    */
    void init(u32 blockSize, u32 numBlocks);

    void initFromMemory(u32 size, void* mem, u32 numBlocks);
    void initFromMemory(u32 size, void* mem, u32 numBlocks, u32 alignment);

    virtual void* allocate(u32 size);
    virtual void* allocate(u32 size, u32 alignment);

    /*!
        \brief Allocates a single block.
        \return Pointer to the new block.
    */
    void* allocateBlock();

    virtual void free(const void* pointer);
    virtual void reset();

};

} /* namespace DE */

#endif /* POOLALLOCATOR_H_ */
