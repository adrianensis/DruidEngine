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

    // void allocationLoop(u32 length);
    void allocate(u32 elementSize, u32 alignment);
    void copy(const BaseList& other);

private:

    BaseNode* newNode();
    void freeNode(BaseNode* node);
    static u32 smNodeSize;

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
        bool isNull();
        bool hasNext();
        void next();
        BaseIterator getNext();
        bool hasPrev();
        void prev();
        BaseIterator getPrev();
        void* get();
        void set(void* element);
        bool isReverse();
        void setReverse(bool isReverse);

        bool operator==(const BaseIterator& rhs) const {
            return this->mNode == rhs.mNode;
        }

        bool operator!=(const BaseIterator& rhs) const {
            return !((*this) == rhs);
        }

        BaseIterator& operator=(const BaseIterator& rhs) {
            // if (this == &rhs) return *this; // handle self assignment
            //assignment operator
            this->mNode = rhs.mNode;
            this->mReverse = rhs.mReverse;
            return (*this);
        }
    };

    BaseList ();
    virtual ~BaseList ();

    void init(u32 elementSize);

    BaseIterator getIterator() const;
    BaseIterator getRevIterator() const;
    BaseIterator getFirst() const;
    BaseIterator getLast() const;
    bool isEmpty() const;
    void pushFront(void* element);
    void pushBack(void* element);
    void* popFront();
    void* popBack();
    void* get(u32 index) const;
    void set(u32 index, void* element);
    void swap(u32 index1, u32 index2);
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
