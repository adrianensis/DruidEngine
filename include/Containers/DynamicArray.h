#ifndef DYNAMICARRAY_H_
#define DYNAMICARRAY_H_

#include "Container.h"
#include "Array.h"
#include "List.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"
#include <math.h> /* ceil, max */

namespace DE {

/*!
    \brief DynamicArray of elements. Fixed size.
    \tparam Elements class.
*/
template <class T>
class DynamicArray : public Container {

private:

    static const u32 mMinSize = 100;

    List<Array<T>*>* mArrays;

public:

    /*!
        \brief Default Constructor.
    */
    DynamicArray() : Container(){};

    /*!
        \brief Destructor.
    */
    virtual ~DynamicArray(){};

    /*!
        \brief Copy Constructor.
        \param other Other DynamicArray.
    */
    void init(const DynamicArray<T>& other){
        mArrays = DE::allocate< List<Array<T>*> >(*mAllocator); // TODO: change mAllocator for Memory::allocate();
        mArrays->init(*(other.mArrays));
    };

    /*!
        \brief Copy Constructor.
        \param other Other Array.
    */
    void init(const Array<T>& other){
        u32 otherLength = other.getLength();

        DynamicArray::init(otherLength, other.getAlignment());

        u32 otherOffset = 0;

        auto it = mArrays->getIterator();

        for (; (!it.isNull()) && otherOffset < otherLength; it.next()){
            Array<T>* array = it.get();
            for (u32 i = 0; i < array->getLength() && otherOffset < otherLength; i++){
                (*array)[i] = other[i];
                otherOffset++;
            }
        }
    };

    /*!
        \brief Constructor from raw array.
        \param rawArray The raw array.
        \param length The length of the raw array.
    */
    void init(const void* rawArray, u32 length) {
        DynamicArray::init(rawArray, length, 1);
    };

    /*!
        \brief Constructor from raw array. Aligned.
        \param rawArray The raw array.
        \param length The length of the raw array.
        \param alignment Bytes alignment.
    */
    void init(const void* rawArray, u32 length, u32 alignment) {
        Array<T>* array = DE::allocate<Array<T>>(*mAllocator, alignment);
        array->init(rawArray, length);
        DynamicArray::init(*array);
    };

    /*!
        \brief Constructor.
    */
    void init() {
        DynamicArray::init(0);
    };

    /*!
        \brief Constructor.
        \param length Length of the array.
    */
    void init(u32 length) {
        DynamicArray::init(length, 1);
    };

    /*!
        \brief Constructor. Aligned.
        \param length Length of the array.
        \param alignment Bytes alignment.
    */
    void init(u32 length, u32 alignment) {
        Container::init(length, sizeof(T), alignment);

        // list of arrays
        mArrays = DE::allocate< List<Array<T>*> >(*mAllocator); // TODO: change mAllocator for Memory::allocate();
        mArrays->init();

        // how many arrays are needed.
        u32 arrayCount = ceil(length/mMinSize);

        for (u32 i = 0; i <= arrayCount; i++) {
            Array<T>* newArray = DE::allocate<Array<T>>(*mAllocator, mAlignment);
            newArray->init(mMinSize);

            mArrays->pushBack(newArray);
        }
    };

    /*!
        \brief Fill the array with the same element.
        \param element The element.
    */
    void fill(T element){

        auto it = mArrays->getIterator();

        for (; it.isNull(); it.next()){
            Array<T>* array = it.get();
            for (u32 i = 0; i < array->getLength(); i++)
                (*array)[i] = element;
        }
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param index Index (of the destiny array) from which to paste the other array.
    */
    void put(const Array<T>& other, u32 index){
        // ASSERT(other.getLength() <= this->getLength() - index, "Not enough space for put array.");
        // BaseArray::put(other.mStart, index, other.getLength()*mElementSize);
    };

    /*!
        \brief Copy an array into other.
        \param other Other DynamicArray.
        \param index Index (of the destiny array) from which to paste the other array.
    */
    void put(const DynamicArray<T>& other, u32 index){
        // ASSERT(other.getLength() <= this->getLength() - index, "Not enough space for put array.");
        // BaseArray::put(other.mStart, index, other.getLength()*mElementSize);
    };

    /*!
        \brief Copy an array into other.
        \param other Other DynamicArray.
        \param index Index (of the destiny array) from which to paste the other array.
        \param length Amount of element of the other array to be copied.
    */
    void put(const void* rawArray, u32 index, u32 length){
        Array<T>* array = DE::allocate<Array<T>>(*mAllocator, mAlignment);
        array->init(rawArray, length);
        DynamicArray::put(*array, index);
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t i) {
        ASSERT(i >= 0, "Index must be greater than 0.");

        u32 realIndex = i % mMinSize;
        u32 arrayIndex = ceil(i/mMinSize);

        return (*(mArrays->get(arrayIndex)))[realIndex];
    }

    /*!
        \brief Read only.
        \param i Index.
        \return Element reference.
    */
	T operator[](const size_t i) const {
        ASSERT(i >= 0, "Index must be greater than 0.");

        u32 realIndex = i % mMinSize;
        u32 arrayIndex = ceil(i/mMinSize);

        return (*(mArrays->get(arrayIndex)))[realIndex];
	}

    /*!
        \param index The index.
        \return Element at index.
    */
    T get(u32 index) const{
        return (*this)[index];
    };

    /*!
        \brief Sets element at index.
        \param element The element.
        \param element The element.
    */
    void set(u32 index, T element){
        // resize
        if(index > mArrays->getLength()*mMinSize){
            Array<T>* newArray = DE::allocate<Array<T>>(*mAllocator, mAlignment); // TODO: change mAllocator for Memory::allocate();
            newArray->init(mMinSize);

            mArrays->pushBack(newArray);
        }

        mLength = max(index + 1, mLength); // save the max index accessed

        (*this)[index] = element;
    };


};

} /* namespace DE */

#endif /* DYNAMICARRAY_H_ */
