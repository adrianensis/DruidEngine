#ifndef BASELIST_H_
#define BASELIST_H_

#include "Container.h"
#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

class BaseList : public Container {

friend class BaseIterator;

protected:

  class BaseNode {

  public:
    BaseNode* mNext;
    BaseNode* mPrev;
    void* mElement;
    BaseNode ();
    virtual ~BaseNode ();
    void init();
    void init(void* element);
  };

  BaseNode* mFirst;
  BaseNode* mLast;

  // void allocationLoop(const u32 length);
  void allocate(const u32 elementSize, const u32 alignment);
  void copy(const BaseList& other);

private:

  BaseNode* newNode();
  void freeNode(BaseNode* node);
  static const u32 smNodeSize;

protected:

  class BaseIterator {
  friend class BaseList;
  private:
    BaseNode* mNode;
    bool mReverse;
  public:
    BaseIterator ();
    BaseIterator(const BaseIterator& other);
    virtual ~BaseIterator ();
    void init(BaseNode* start);
    bool hasNext();
    void next();
    void* get();
    bool isReverse();
    void setReverse(bool isReverse);
  };

  BaseList ();
  virtual ~BaseList ();

  void init(const u32 elementSize);

  BaseIterator getIterator() const;
  BaseIterator getRevIterator() const;
  bool isEmpty();
  void pushFront(void* element);
  void pushBack(void* element);
  void* popFront();
  void* popBack();
  void* get(u32 index) const;
  void remove(u32 index);
  void remove(BaseIterator& it);
  void insert(u32 index, void* element);
  void insert(BaseIterator& it, void* element);

public:

  static const u32& getNodeSize() {return smNodeSize; };

  void clear();
};


} /* namespace DE */

#endif /* BASELIST_H_ */
