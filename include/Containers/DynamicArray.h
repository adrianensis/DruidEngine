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

    static const u32 smMinSize = 100;
    Array<T>* mCache = nullptr;
    u32 mCacheIndex = 0;
    List<Array<T>*>* mArrays;

    void updateCache(u32 arrayIndex){
        if(arrayIndex != mCacheIndex || mCache == nullptr){
            mCache = mArrays->get(arrayIndex);
            mCacheIndex = arrayIndex;
        }
    }

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
        Container::init(other.mLength, other.mElementSize, other.mAlignment);
        mArrays = DE::allocate< List<Array<T>*> >(*mAllocator); // TODO: change mAllocator for Memory::allocate();
        mArrays->init(*(other.mArrays));
    };

    /*!
        \brief Copy Constructor.
        \param other Other Array.
    */
    void init(const Array<T>& other){
        u32 otherLength = other.getLength();
        u32 otherOffset = 0;

        DynamicArray::init(otherLength, other.getAlignment());

        auto it = mArrays->getIterator();

        for (; (!it.isNull()) && otherOffset < otherLength; it.next()){
            Array<T>* array = it.get();

            if(otherLength < smMinSize)
                array->put(other, 0, 0);
            else
                array->put(other, 0, otherOffset, smMinSize - 1);

            otherOffset += smMinSize;
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
        u32 arrayCount = ceil(length/smMinSize) + 1;

        for (u32 i = 0; i < arrayCount; i++) {
            Array<T>* newArray = DE::allocate<Array<T>>(*mAllocator, mAlignment);
            newArray->init(smMinSize);

            mArrays->pushBack(newArray);
        }
    };

    /*!
        \brief Fill the array with the same element.
        \param element The element.
    */
    void fill(T element){

        auto it = mArrays->getIterator();

        for (; !it.isNull(); it.next()){
            Array<T>* array = it.get();
            array->fill(element);
        }
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
        \param length Amount of element of the other array to be copied.
    */
    void put(const Array<T>& other, u32 destinyIndex, u32 sourceIndex, u32 length){
        ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
        ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
        ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");

        for (int i = 0; i < length; ++i)
            (*this)[destinyIndex + i] = other[sourceIndex + i];
    };

    /*!
        \brief Copy an array into other.
        \param other Other DynamicArray.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
        \param length Amount of element of the other array to be copied.
    */
    void put(DynamicArray<T>& other, u32 destinyIndex, u32 sourceIndex, u32 length){
        ASSERT(sourceIndex >= 0, "sourceIndex must be greater than 0.");
        ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");

        for (int i = 0; i < length; ++i)
            (*this)[destinyIndex + i] = other[sourceIndex + i];
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(const Array<T>& other, u32 destinyIndex, u32 sourceIndex){
        DynamicArray::put(other, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Copy an array into other.
        \param other Other DynamicArray.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(DynamicArray<T>& other, u32 destinyIndex, u32 sourceIndex){
        DynamicArray::put(other, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t i) {
        ASSERT(i >= 0, "Index must be greater than 0.");

        u32 realIndex = i % smMinSize;
        u32 arrayIndex = ceil(i/smMinSize);

        updateCache(arrayIndex);
        return (*mCache)[realIndex];
    }

    /*!
        \param index The index.
        \return Element at index.
    */
    T get(u32 index){
        return (*this)[index];
    };

    /*!
        \brief Sets element at index.
        \param element The element.
        \param element The element.
    */
    void set(u32 index, T element){
        // resize
        if(index >= mArrays->getLength()*smMinSize){
            Array<T>* newArray = DE::allocate<Array<T>>(*mAllocator, mAlignment); // TODO: change mAllocator for Memory::allocate();
            newArray->init(smMinSize);

            mArrays->pushBack(newArray);
        }

        mLength = max(index + 1, mLength); // save the max index accessed

        (*this)[index] = element;
    };


};

} /* namespace DE */

#endif /* DYNAMICARRAY_H_ */
