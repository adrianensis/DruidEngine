#ifndef LIST_H_
#define LIST_H_

#include "BaseList.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"

namespace DE {

/*!
    \brief List of elements.
    \tparam Elements class.
*/
template <class T>
class List : public BaseList {

public:

    /*!
    \brief Iterator. Allows iterate over elements.
    */
    class Iterator : public BaseIterator {

    public:

        /*!
            \brief Default Constructor.
        */
        Iterator () : BaseIterator(){ };

        /*!
            \brief Copy Constructor.
            \param other Other Iterator.
        */
        Iterator(const BaseIterator& other) : BaseIterator(other){ };

        /*!
            \brief Destructor.
        */
        virtual ~Iterator (){ };

        /*!
            \return Element reference.
        */
        virtual T& get(){
            return *(static_cast<T*>(BaseIterator::get()));
        };

        /*!
            \brief Sets the iterator element.
            \param element The element.
        */
        virtual void set(T& element){
            BaseIterator::set(static_cast<void*>(&element));
        };

        /*!
            \return The next Iterator.
        */
        Iterator getNext(){
            return BaseIterator::getNext();
        };

        /*!
            \return The previous Iterator.
        */
        Iterator getPrev(){
            return BaseIterator::getPrev();
        };

    };

private:

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

    /*!
        \brief Default Constructor.
    */
    List() : BaseList(){};

    /*!
        \brief Destructor.
    */
    virtual ~List(){};

    /*!
        \brief Copy Constructor.
        \param other Other List.
    */
    void init(const List& other){
        BaseList::copy(other);
    };

    /*!
        \brief Constructor. Empty List.
    */
    void init() {
        BaseList::init(sizeof(T));
    };

    /*!
        \return A Iterator.
    */
    Iterator getIterator() const {
        return Iterator(BaseList::getIterator());
    };

    /*!
        \return A reverse Iterator.
    */
    Iterator getRevIterator() const {
        return Iterator(BaseList::getRevIterator());
    };

    /*!
        \brief Push element at front.
        \param element The element.
    */
    void pushFront(T element){
        BaseList::pushFront(&element);
    };

    /*!
        \brief Push element at back.
        \param element The element.
    */
    void pushBack(T element){
        BaseList::pushBack(&element);
    };

    /*!
        \return Element from front.
    */
    T popFront(){
        return *(static_cast<T*>(BaseList::popFront()));
    };

    /*!
        \return Element from back.
    */
    T popBack(){
        return *(static_cast<T*>(BaseList::popBack()));
    };

    /*!
        \param index The index.
        \return Element at index.
    */
    T get(u32 index) const{
        return *(static_cast<T*>(BaseList::get(index)));
    };

    /*!
        \brief Sets element at index.
        \param element The element.
        \param element The element.
    */
    void set(u32 index, T element){
        BaseList::set(index, static_cast<void*>(&element));
    };

    /*!
        \brief Swaps two elements.
        \param index1 The first index.
        \param index2 The second index.
    */
    void swap(u32 index1, u32 index2){
        BaseList::swap(index1, index2);
    };

    /*!
        \brief Removes an element.
        \param index The index.
    */
    void remove(u32 index){
        BaseList::remove(index);
    };

    /*!
        \brief Removes an element from Iterator.
        \param it The Iterator.
    */
    void remove(Iterator& it){
        BaseList::remove(it);
    };

    /*!
        \brief Inserts an element at index.
        \param index The index.
        \param element The element.
    */
    void insert(u32 index, T element){
        BaseList::insert(index, &element);
    };

    /*!
        \brief Inserts an element from Iterator.
        \param it The Iterator.
        \param element The element.
    */
    void insert(Iterator& it, T element){
        BaseList::insert(it, &element);
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
