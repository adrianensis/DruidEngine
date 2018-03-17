#ifndef DYNAMICARRAY_H_
#define DYNAMICARRAY_H_

#include "Container.h"
#include "Array.h"
#include "List.h"
#include "Basic.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"
#include "MathUtils.h"

namespace DE {

/*!
    \brief DynamicArray of elements. Fixed size.
    \tparam Elements class.
*/
template <class T>
DE_CLASS(DynamicArray) DE_CLASS_EXTENDS public Container {

template <class K, class V>
friend class HashMap; // Friend Class

private:

    static const u32 smMinSize = 100;
    mutable Array<T>* mCache = nullptr;
    mutable u32 mCacheIndex = 0;
    List<Array<T>*>* mArrays;

    void _updateCache(const u32 arrayIndex) const{ // const method, but mutable variables.
        if(arrayIndex != mCacheIndex || mCache == nullptr){
            mCache = mArrays->get(arrayIndex);
            mCacheIndex = arrayIndex;
        }
    }

    T& _get(const u32 i) const{
        DE_ASSERT(i >= 0, "Index must be greater than 0.");

        u32 realIndex = i % smMinSize;
        u32 arrayIndex = ceil(i/smMinSize);

        DynamicArray::_updateCache(arrayIndex);
        return (*mCache)[realIndex];
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
    void init(const void* rawArray, const u32 length) {
        DynamicArray::init(rawArray, length, 1);
    };

    /*!
        \brief Constructor from raw array. Aligned.
        \param rawArray The raw array.
        \param length The length of the raw array.
        \param alignment Bytes alignment.
    */
    void init(const void* rawArray, const u32 length, const u32 alignment) {
        Container::setAllocator(&Memory::getGlobal());
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
    void init(const u32 length) {
        DynamicArray::init(length, 1);
    };

    /*!
        \brief Constructor. Aligned.
        \param length Length of the array.
        \param alignment Bytes alignment.
    */
    void init(const u32 length, const u32 alignment) {
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
    void fill(const T element){

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
    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length){
        DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
        DE_ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
        DE_ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");

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
    void put(const DynamicArray<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length){
        DE_ASSERT(sourceIndex >= 0, "sourceIndex must be greater than 0.");
        DE_ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");

        for (int i = 0; i < length; ++i)
            (*this)[destinyIndex + i] = other[sourceIndex + i];
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex){
        DynamicArray::put(other, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Copy an array into other.
        \param other Other DynamicArray.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(const DynamicArray<T>& other, const u32 destinyIndex, const u32 sourceIndex){
        DynamicArray::put(other, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t index) {
        return DynamicArray::_get(index);
    };

    /*!
        \brief Read only.
        \param i Index.
        \return Element reference.
    */
	T operator[](const size_t index) const {
        return DynamicArray::_get(index);
	}

    /*!
        \param index The index.
        \return Element at index.
    */
    T get(const u32 index) const{
        return DynamicArray::_get(index);
    };

    /*!
        \brief Sets element at index.
        \param element The element.
        \param element The element.
    */
    void set(const u32 index, const T element){
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
