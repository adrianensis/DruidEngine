#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "IArray.h"

namespace DE {

/*!
    \brief Array of elements. Fixed size.
    \tparam Elements class.
*/
template <class T>
class Array : public IArray<T>, public BaseArray {

private:

    T* mTStart;

public:

    /*!
        \brief Default Constructor.
    */
    Array() : IArray<T>(), BaseArray(){

    };

    /*!
        \brief Destructor.
    */
    ~Array() override = default;

    /*!
        \brief Copy Constructor.
        \param other Other Array.
    */
    void init(const Array<T>& other){
        BaseArray::copy(other);
        mTStart = static_cast<T*>(mStart);
    };


    void fill(const T element) override {
        for (u32 i = 0; i < ISequentialContainer<T>::mLength; i++)
            mTStart[i] = element;
    };

    void init(const void* rawArray, const u32 length) override {
        BaseArray::init(rawArray, length, sizeof(T));
        mTStart = static_cast<T*>(mStart);
    };

    void init(const void* rawArray, const u32 length, const u32 alignment) override {
        BaseArray::init(rawArray, length, sizeof(T), alignment);
        mTStart = static_cast<T*>(mStart);
    };

    void init(const u32 length) override {
        BaseArray::init(length, sizeof(T));
        mTStart = static_cast<T*>(mStart);
    };

    void init(const u32 length, const u32 alignment) override {
        BaseArray::init(length, sizeof(T), alignment);
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex) {
        DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
        DE_ASSERT(destinyIndex >= 0 && destinyIndex < ISequentialContainer<T>::getLength(), "destinyIndex is out of bounds.");
        DE_ASSERT(other.getLength() - sourceIndex <= ISequentialContainer<T>::getLength() - destinyIndex, "Not enough space to put array.");
        BaseArray::put(other.mStart, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
        \param length Amount of element of the other array to be copied.
    */
    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) {
        DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
        DE_ASSERT(destinyIndex >= 0 && destinyIndex < ISequentialContainer<T>::getLength(), "destinyIndex is out of bounds.");
        DE_ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
        DE_ASSERT(length <= ISequentialContainer<T>::getLength() - destinyIndex, "Not enough space to put array.");
        BaseArray::put(other.mStart, destinyIndex, sourceIndex, length);
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t index) {
        DE_ASSERT(index >= 0 && index < ISequentialContainer<T>::getLength(), "Index out of bounds.");
        return mTStart[index];
    }

    /*!
        \brief Read only.
        \param i Index.
        \return Element reference.
    */
	T operator[](const size_t index) const {
		DE_ASSERT(index >= 0 && index < ISequentialContainer<T>::getLength(), "Index out of bounds.");
		return mTStart[index];
	}

    T get(const u32 index) const override {
        DE_ASSERT(index >= 0 && index < ISequentialContainer<T>::getLength(), "Index out of bounds.");
		return mTStart[index];
    };

    void set(const u32 index, const T element) override {
        DE_ASSERT(index >= 0 && index < ISequentialContainer<T>::getLength(), "Index out of bounds.");
        mTStart[index] = element;
    };

    void clear() override {
        BaseArray::clear();
    }

};

} /* namespace DE */

#endif /* ARRAY_H_ */
