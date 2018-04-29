#ifndef LIST_H_
#define LIST_H_

#include "SequentialContainer.h"
#include "Allocator.h"
#include "Basic.h"

namespace DE {

template <class T>
class List : public SequentialContainer<T>{

friend class Iterator;

private:

    class Node {

    public:
        Node* mNext;
        Node* mPrev;
        T mElement;

        Node() {
            Node::init();
        };

        ~Node() { };

        void init() {
            mNext = nullptr;
            mPrev = nullptr;
            //mElement = nullptr;
        };

        void init(const T element) {
            Node::init();
            mElement = element;
        };
    };

    static const u32 smNodeSize = sizeof(Node);

    Node* mFirst;
    Node* mLast;

    Node* newNode(){
        Node* node = static_cast<Node*>(BaseContainer::mAllocator->allocate(smNodeSize));
        return node;
    };

    void freeNode(Node* node){
        BaseContainer::mAllocator->free(node);
    };

    static u8 defaultComparator(const T& a, const T& b){
        return (a < b) ? 1 : (b < a) ? 2 : 0;
    };

    void qsort(u8 (*comparator)(const T& a, const T& b), i32 left, i32 right){

        i32 length = right - left + 1;

        if(length > 1){

            T pivot = this->get((left + length)/2);

            while(left <= right){

                while(comparator(this->get(left), pivot) == 1)
                    left++;

                while(comparator(this->get(right), pivot) == 2)
                    right--;

                if(left <= right){
                    this->swap(left,right);
                    left++;
                    right--;
                }
            }

            qsort(comparator, 0, right);
            qsort(comparator, left, length-1);

        }
    };

public:

    class Iterator {

    friend class List;

    private:

        Node* mNode;
        bool mReverse;

        void init(Node* start) {
            mNode = start;
            mReverse = false;
        };

    public:

        Iterator() {
            mNode = nullptr;
        };

        Iterator(const Iterator& other) {
            mNode = other.mNode;
            mReverse = other.mReverse;
        };

        ~Iterator() {

        };

        bool isNull() const{
            return mNode == nullptr;
        };

        bool hasNext() {
            DE_ASSERT(mNode != nullptr, "Node is null.");

            if(mReverse)
                return (mNode->mPrev != nullptr);
            else
                return (mNode->mNext != nullptr);
        };

        void next() {
            DE_ASSERT(mNode != nullptr, "Node is null.");

            if(mReverse)
                mNode = mNode->mPrev;
            else
                mNode = mNode->mNext;
        };

        Iterator getNext(){
            DE_ASSERT(mNode != nullptr, "Node is null.");

            Iterator it;

            if(mReverse)
                it.init(mNode->mPrev);
            else
                it.init(mNode->mNext);

            return it;
        };

        bool hasPrev(){
            DE_ASSERT(mNode != nullptr, "Node is null.");

            if(mReverse)
                return (mNode->mNext != nullptr);
            else
                return (mNode->mPrev != nullptr);
        };

        void prev(){
            DE_ASSERT(mNode != nullptr, "Node is null.");

            if(mReverse)
                mNode = mNode->mNext;
            else
                mNode = mNode->mPrev;
        };

        Iterator getPrev(){
            DE_ASSERT(mNode != nullptr, "Node is null.");

            Iterator it;

            if(mReverse)
                it.init(mNode->mNext);
            else
                it.init(mNode->mPrev);

            return it;
        };

        T& get() const {
            DE_ASSERT(mNode != nullptr, "Node is null.");

            return mNode->mElement;
        };

        void set(T element){
            mNode->mElement = element;
        };

        bool isReverse() const {
            return mReverse;
        };

        void setReverse(bool isReverse){
            mReverse = isReverse;
        };

        bool operator==(const Iterator& rhs) const {
            return this->mNode == rhs.mNode &&
              this->mReverse == rhs.mReverse;
        }

        bool operator!=(const Iterator& rhs) const {
            return !((*this) == rhs);
        }

        Iterator& operator=(const Iterator& rhs) {
            if (this == &rhs) return *this; // handle self assignment
            //assignment operator
            this->mNode = rhs.mNode;
            this->mReverse = rhs.mReverse;
            return (*this);
        }
    };

private:

    void allocate(u32 elementSize, const u32 alignment) {
        BaseContainer::init(0, elementSize, alignment); // BaseContainer::mLength = 0
    };

    void _init(u32 elementSize) {
        List::allocate(elementSize, 1);
        resetCache();
    };

    mutable Iterator mLastAccessedIt;
    mutable i32 mLastAccessedIndex;

    void resetCache(){
      mLastAccessedIndex = -1;
    }

    T& randomAccessOperator(const u32 index) const{
      DE_ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

      const i32 signedIndex = static_cast<i32>(index);

      // first time, init the index.
      if(mLastAccessedIndex == -1){
        mLastAccessedIt = List::getIterator();
        mLastAccessedIndex = 0;
      }

      // if i == last, just return last accessed index

       if(signedIndex == mLastAccessedIndex+1){ // if i == last+1
        if(mLastAccessedIt.hasNext()){
          mLastAccessedIt.next();
          mLastAccessedIndex++;
        }
      }else if(signedIndex == mLastAccessedIndex-1){ // if i == last-1
        if(mLastAccessedIt.hasPrev()){
          mLastAccessedIt.prev();
          mLastAccessedIndex--;
        }
      }else if(signedIndex != mLastAccessedIndex){ // if i != last
        mLastAccessedIndex = 0;
        mLastAccessedIt = List::getIterator();

        for (; mLastAccessedIndex < signedIndex && mLastAccessedIt.hasNext(); mLastAccessedIt.next())
            mLastAccessedIndex++;
      }

      return mLastAccessedIt.get();
    };

    void checkPut(const SequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) override {
      DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
      DE_ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
      DE_ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
    };

public:

    List() : SequentialContainer<T>() {
        mFirst = nullptr;
        mLast = nullptr;
    };

    ~List() {
        // NOTE: user must free memory externally, by using the choosen allocator.
    };

    /*!
        \brief Copy Constructor.
        \param other Other List.
    */
    void init(const List& other){
        this->_init(other.mElementSize);
        Iterator it = other.getIterator();

        for (; it.hasNext(); it.next())
            this->pushBack(it.get());

        this->pushBack(it.get());
    };

    /*!
        \brief Constructor. Empty List.
    */
    void init() {
        List::_init(sizeof(T));
    };

    void init(const void* rawArray, const u32 length) override {
        List::init();
        const T* typedArray = static_cast<const T*>(rawArray);

        for (u32 i = 0; i < length; i++)
          List::pushBack(typedArray[i]);
    };

    void init(const void* rawArray, const u32 length, const u32 alignment) override {
        List::init(rawArray, length);
    };

    void init(const u32 length) override {
        List::init();
    };

    void init(const u32 length, const u32 alignment) override {
        List::init();
    };

    void fill(const T element) override {
        if( ! List::isEmpty()){
            Iterator it = List::getIterator();
            for (; it.hasNext(); it.next())
                it.set(element);

            it.set(element);
        }
    };

    void put(const SequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) override {
        resetCache();
        this->checkPut(other, destinyIndex, sourceIndex, length);

        u32 i = 0;
        Iterator it = List::getIterator();

        if( ! List::isEmpty()){
            for (; i < destinyIndex && it.hasNext(); it.next())
                i++;

            i = 0;

            // update nodes
            for (; i < length && it.hasNext(); it.next()){
                it.set(other.get(i));
                i++;
            }
        }

        // create new nodes
        for (; i < length; i++) {
          List::pushBack(other.get(i));
        }
    };

    Iterator getIterator() const{
        return List::getFirst();
    };

    Iterator getRevIterator() const{
        Iterator it;
        it.init(this->mLast);
        it.setReverse(true);
        return it;
    };

    Iterator getFirst() const {
        Iterator it;
        it.init(this->mFirst);
        return it;
    };

    Iterator getLast() const {
        Iterator it;
        it.init(this->mLast);
        return it;
    };

    bool isEmpty() const{
        return BaseContainer::mLength == 0;
    };

    void clear() override {
      if( ! List::isEmpty()){
          Iterator it = List::getIterator();
          for (; it.hasNext(); it.next())
              List::remove(it);

          List::remove(it); // remove last
      }

      mFirst = nullptr;
      mLast = nullptr;

    };

    void pushFront(T element){
      resetCache();
      Node* node = newNode();
      node->init(element);

      if( ! List::isEmpty()){
          mFirst->mPrev = node;
          node->mNext = mFirst;
      }else
          mLast = node; // if is empty then also sets mLast

      mFirst = node;

      BaseContainer::mLength++;

    };

    void pushBack(T element){
      resetCache();
      Node* node = newNode();
      node->init(element);

      if( ! List::isEmpty()){
          mLast->mNext = node;
          node->mPrev = mLast;
      }else
          mFirst = node; // if is empty then also sets mFirst

      mLast = node;

      BaseContainer::mLength++;

    };

    T popFront(){
        resetCache();
        T* element = nullptr;

        if( ! List::isEmpty()){
            BaseContainer::mLength--;

            element = &mFirst->mElement;
            Node* old = mFirst;

            if(! List::isEmpty()){
                mFirst = mFirst->mNext;
                mFirst->mPrev = nullptr;
            }else{ // if len = 0
                mFirst = nullptr;
                mLast = nullptr;
            }

            List::freeNode(old);
        }

        return *element;
    };

    T popBack(){
        resetCache();
        T* element = nullptr;

        if( ! List::isEmpty()){
            BaseContainer::mLength--;

            element = &mLast->mElement;
            Node* old = mLast;

            if(! List::isEmpty()){
                mLast = mLast->mPrev;
                mLast->mNext = nullptr;
            }else{ // if len = 0
                mFirst = nullptr;
                mLast = nullptr;
            }

            List::freeNode(old);
        }

        return *element;
    };

    T get(const u32 index) const{
      return List::randomAccessOperator(index);
    };

    void set(const u32 index, const T element){
        DE_ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

        u32 i = 0;
        Iterator it = List::getIterator();

        for (; i < index && it.hasNext(); it.next())
            i++;

        it.set(element);
    };

    void swap(u32 index1, u32 index2){
        u32 i = 0;
        Iterator it1 = List::getIterator();
        Iterator it2 = List::getIterator();

        for (; i < index1 && it1.hasNext(); it1.next())
            i++;

        T element1 = it1.get();

        i = 0;

        for (; i < index2 && it2.hasNext(); it2.next())
            i++;

        T element2 = it2.get();

        it1.set(element2);
        it2.set(element1);
    };

    void remove(u32 index){
        resetCache();
        DE_ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

        if( ! List::isEmpty()){
            // BaseContainer::mLength--;

            u32 i = 0;
            Iterator it = List::getIterator();

            for (; i < index && it.hasNext(); it.next())
                i++;

            List::remove(it);
        }

    };

    void remove(Iterator& it){
        resetCache();
        if( ! List::isEmpty()){
            BaseContainer::mLength--;

            Node* prev = it.mNode->mPrev;
            Node* next = it.mNode->mNext;

            // check First and Last
            if(it.mNode == mFirst){
                mFirst = next;
            }

            if(it.mNode == mLast){
                mLast = prev;
            }

            if(prev != nullptr)
                prev->mNext = next;

            if(next != nullptr)
                next->mPrev = prev;

            List::freeNode(it.mNode);
        }

        if(List::isEmpty()){
            mFirst = nullptr;
            mLast = nullptr;
        }

    };

    void insert(u32 index, T element){
        resetCache();
        DE_ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

        // BaseContainer::mLength++;

        // TODO refactor loop
        u32 i = 0;
        Iterator it = List::getIterator();

        for (; i < index && it.hasNext(); it.next())
            i++;

        List::insert(it,element);
    };

    void insert(Iterator& it, T element){
        resetCache();
        if(it.mNode != mFirst){
            Node* node = newNode();
            node->init(element);

            // this function inserts new node before the current node (it.mNode)
            it.mNode->mPrev->mNext = node;

            node->mPrev = it.mNode->mPrev;
            node->mNext = it.mNode;

            BaseContainer::mLength++;
        }else
            List::pushFront(element);
    };

    /*!
        \brief Sorts with default comparator.
    */
    void sort(){
        sort(defaultComparator);
    };

    /*!
        \brief Sorts custom comparator.
        \param comparator Function with this form: u8 comparator(const T& a, const T& b).
    */
    void sort(u8 (*comparator)(const T& a, const T& b)){
        qsort(comparator, 0, this->getLength()-1);
    };

};


} /* namespace DE */

#endif /* LIST_H_ */
