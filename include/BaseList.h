#ifndef BASELIST_H_
#define BASELIST_H_

#include "Container.h"
#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

class BaseList : public Container {


protected:

  class BaseNode {

  public:
    BaseNode* mNext;
    BaseNode* mPrev;
    void* mElement;
    BaseNode ();
    virtual ~BaseNode ();
    void init();
  };

  BaseNode* mFirst;
  BaseNode* mLast;

  void allocationLoop(const u32 length);
  virtual void* allocate(const u32 length, const u32 elementSize, Allocator* allocator);
  virtual void* allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

  // push_back, push_front, delete, insert.

public:

  BaseList ();
  virtual ~BaseList ();

  virtual void* init(const u32 length, const u32 elementSize, Allocator* allocator);
  virtual void* init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

};

} /* namespace DE */

#endif /* BASELIST_H_ */
