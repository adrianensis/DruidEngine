#ifndef BASELIST_H_
#define BASELIST_H_

#include "Container.h"
#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

class BaseList : public Container {

friend class Iterator;

protected:

  class BaseNode {

  public:
    BaseNode* mNext;
    BaseNode* mPrev;
    void* mElement;
    BaseNode ();
    virtual ~BaseNode ();
    virtual void init();
    virtual void init(void* element);
  };

  BaseNode* mFirst;
  BaseNode* mLast;

  // void allocationLoop(const u32 length);
  virtual void allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

private:

  static const u32 smNodeSize;
  BaseNode* newNode();

public:

  class BaseIterator {
  friend class BaseList;
  private:
    BaseNode* mNode;
    bool mReverse;
  public:
    BaseIterator ();
    virtual ~BaseIterator ();
    void init(BaseNode* start);
    virtual bool hasNext();
    virtual void next();
    virtual void* get();
    virtual bool isReverse();
    virtual void setReverse(bool isReverse);
  };

  BaseList ();
  virtual ~BaseList ();

  virtual void init(const u32 length, const u32 elementSize, Allocator* allocator);
  virtual void init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

  virtual BaseIterator getIterator() const;
  virtual BaseIterator getRevIterator() const;
  virtual bool isEmpty();
  virtual void pushFront(void* element);
  virtual void pushBack(void* element);
  virtual void* popFront();
  virtual void* popBack();
  virtual void* get(u32 index) const;
  virtual void remove(u32 index);
  virtual void remove(BaseIterator& it);
  virtual void insert(u32 index, void* element);
  virtual void insert(BaseIterator& it, void* element);
};

} /* namespace DE */

#endif /* BASELIST_H_ */
