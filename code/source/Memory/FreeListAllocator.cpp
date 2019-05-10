#include "FreeListAllocator.h"

namespace DE {

FreeListAllocator::Block::Block(){

}

FreeListAllocator::Block::~Block(){

}

void FreeListAllocator::Block::init(void* unalignedAddress, u32 size){
  this->unalignedAddress = unalignedAddress;
  this->size = size;
}

FreeListAllocator::Block FreeListAllocator::allocateBlock(u32 size){
  auto it = mFreeBlocks->getIterator();

  bool found = false;
  List<Block>::Iterator selectedIt;

  for (; !it.isNull() && !found; it.next()){

    Block b = it.get();

    if(b.size >= size){
      found = true;
      selectedIt = it;
    }
  }

  Block selectedBlock = selectedIt.get();

  mFreeBlocks->remove(selectedIt);

  Block usedBlock;
  usedBlock.init(selectedBlock.unalignedAddress, size);
  mUsedBlocks->pushBack(usedBlock);

  Block newFreeBlock;
  newFreeBlock.init(selectedBlock.unalignedAddress + size, selectedBlock.size - size);
  mFreeBlocks->pushBack(newFreeBlock);

  return selectedBlock;
}

u32 FreeListAllocator::freeBlock(void* unalignedAddress){
  auto it = mUsedBlocks->getIterator();

  bool found = false;
  List<Block>::Iterator selectedIt;

  for (; !it.isNull() && !found; it.next()){

    Block b = it.get();

    if(b.unalignedAddress == unalignedAddress){
      found = true;
      selectedIt = it;
    }
  }

  Block selectedBlock = selectedIt.get();

  mUsedBlocks->remove(selectedIt);

  mFreeBlocks->pushBack(selectedBlock);

  return selectedBlock.size;
}

FreeListAllocator::FreeListAllocator() : Allocator(){

}

FreeListAllocator::~FreeListAllocator(){

}

void FreeListAllocator::init(u32 size){
  Allocator::init(size);
  FreeListAllocator::reset();
}

void* FreeListAllocator::allocate(u32 size){
  return FreeListAllocator::allocate(size, 1);
}

void* FreeListAllocator::allocate(u32 size, u32 alignment){
  Block block = allocateBlock(size+alignment);
  void* unalignedAddress = block.unalignedAddress;
  return Allocator::allocateAlignedAddress(unalignedAddress, size, alignment);
}

void FreeListAllocator::free(const void* pointer){
  // pointer is an aligned address
  void* unalignedAddress = calculateUnalignedAddress(pointer);
  u32 freeSize = freeBlock(unalignedAddress);

  // reduce mAllocated
  Allocator::setAllocatedSize(Allocator::getAllocatedSize() - freeSize);
}

void FreeListAllocator::reset(){
  Allocator::reset();

  mLinearAllocator.init(std::max(1024.0f*10.0f, mTotalSize*0.01f));
  mUsedBlocks = Allocator::internalAllocate<List<Block>>(&mLinearAllocator);
  mFreeBlocks = Allocator::internalAllocate<List<Block>>(&mLinearAllocator);
  mFreeBlocks->init();
  mUsedBlocks->init();
  mFreeBlocks->setAllocator(&mLinearAllocator);
  mUsedBlocks->setAllocator(&mLinearAllocator);

  Block block;
  block.init(mStart, mTotalSize);
  mFreeBlocks->pushBack(block);
}

} /* namespace DE */
