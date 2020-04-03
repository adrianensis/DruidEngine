#include "FreeListAllocator.h"

namespace DE {

  // ---------------------------------------------------------------------------

FreeListAllocator::Block::Block(){

}

FreeListAllocator::Block::~Block(){

}

// ---------------------------------------------------------------------------

void FreeListAllocator::Block::init(void* unalignedAddress, u32 size){
  this->unalignedAddress = unalignedAddress;
  this->size = size;
  this->blockStatus = BlockStatus::FREE;
  next = nullptr;
}

// ---------------------------------------------------------------------------

void FreeListAllocator::pushBackBlock(Block* block) {
  lastBlock->next = block;
  lastBlock = block;
}

// ---------------------------------------------------------------------------

void FreeListAllocator::pushFrontBlock(Block* block) {
  block->next = firstBlock;
  firstBlock = block;
}

// ---------------------------------------------------------------------------

FreeListAllocator::Block* FreeListAllocator::allocateBlock(u32 size){
  bool found = false;

  Block* it = firstBlock;
  Block* selectedBlock = nullptr;

  while(it && !found){

    if(it->size >= size && it->blockStatus == BlockStatus::FREE){
      found = true;
      selectedBlock = it;
    }

    it = it->next;
  }

  selectedBlock->blockStatus = BlockStatus::INVALID;

  Block* usedBlock = Allocator::internalAllocate<Block>(&mLinearAllocator);
  usedBlock->init(selectedBlock->unalignedAddress, size);
  usedBlock->blockStatus = BlockStatus::USED;

  pushBackBlock(usedBlock); // NOTE : Used blocks are pushed BACK

  Block* newFreeBlock = Allocator::internalAllocate<Block>(&mLinearAllocator);
  newFreeBlock->init(selectedBlock->unalignedAddress + size, selectedBlock->size - size);
  newFreeBlock->blockStatus = BlockStatus::FREE;

  pushFrontBlock(newFreeBlock); // NOTE : Free blocks are pushed FRONT

  return selectedBlock;
}

// ---------------------------------------------------------------------------

u32 FreeListAllocator::freeBlock(void* unalignedAddress){
  bool found = false;

  Block* it = firstBlock;
  Block* selectedBlock = nullptr;

  while(it && !found){

    if(it->unalignedAddress == unalignedAddress && it->blockStatus == BlockStatus::USED){
      found = true;
      selectedBlock = it;
    }

    it = it->next;
  }

  selectedBlock->blockStatus = BlockStatus::FREE;

  return selectedBlock->size;
}

// ---------------------------------------------------------------------------

FreeListAllocator::FreeListAllocator() : Allocator(){

}

// ---------------------------------------------------------------------------

FreeListAllocator::~FreeListAllocator(){

}

// ---------------------------------------------------------------------------

void FreeListAllocator::init(u32 size){
	TRACE();

  Allocator::init(size); TRACE()
  FreeListAllocator::reset();
}

// ---------------------------------------------------------------------------

void* FreeListAllocator::allocate(u32 size){
  return FreeListAllocator::allocate(size, 1);
}

// ---------------------------------------------------------------------------

void* FreeListAllocator::allocate(u32 size, u32 alignment){
  Block* block = allocateBlock(size+alignment);
  void* unalignedAddress = block->unalignedAddress;
  return Allocator::allocateAlignedAddress(unalignedAddress, size, alignment);
}

// ---------------------------------------------------------------------------

void FreeListAllocator::free(const void* pointer){
  // pointer is an aligned address
  void* unalignedAddress = calculateUnalignedAddress(pointer);
  u32 freeSize = freeBlock(unalignedAddress);

  // reduce mAllocatedSize
  Allocator::setAllocatedSize(Allocator::getAllocatedSize() - freeSize);
}

// ---------------------------------------------------------------------------

void FreeListAllocator::flush() {
  Block* lastValid = nullptr;
  Block* it = firstBlock;

  u32 freeCounter = 0;
  const u32 max = 10; // TODO : Move to Settings

  while(it && freeCounter < max){

    Block* next = it->next;

    // If block is VALID
    if(it->blockStatus != BlockStatus::INVALID){
      if(lastValid){
        lastValid->next = it;
      }

      lastValid = it;
    } else {
      Allocator::internalFree(it, &mLinearAllocator);
      freeCounter++;
    }

    it = next;
  }
}

// ---------------------------------------------------------------------------

void FreeListAllocator::reset(){
  Allocator::reset();

  mLinearAllocator.init(std::max(1024.0f*10.0f, mTotalSize*0.1f));

  firstBlock = Allocator::internalAllocate<Block>(&mLinearAllocator);
  firstBlock->init(mStart, mTotalSize);

  lastBlock = firstBlock;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
