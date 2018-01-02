#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"

namespace DE {

/*!
    \brief Array of elements. Fixed size.
    \tparam Elements class.
*/
template <class T>
class Array : public BaseArray {

private:

    T* mTStart;

public:

    /*!
        \brief Default Constructor.
    */
    Array() : BaseArray(){

    };

    /*!
        \brief Destructor.
    */
    virtual ~Array(){};

    /*!
        \brief Copy Constructor.
        \param other Other Array.
    */
    void init(const Array<T>& other){
        BaseArray::copy(other);
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Fill the array with the same element.
        \param element The element.
    */
    void fill(T element){
        for (u32 i = 0; i < mLength; i++)
            mTStart[i] = element;
    };

    /*!
        \brief Constructor from raw array.
        \param rawArray The raw array.
        \param length The length of the raw array.
    */
    void init(const void* rawArray, u32 length) {
        BaseArray::init(rawArray, length, sizeof(T));
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Constructor from raw array. Aligned.
        \param rawArray The raw array.
        \param length The length of the raw array.
        \param alignment Bytes alignment.
    */
    void init(const void* rawArray, u32 length, u32 alignment) {
        BaseArray::init(rawArray, length, sizeof(T), alignment);
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Constructor.
        \param length Length of the array.
    */
    void init(u32 length) {
        BaseArray::init(length, sizeof(T));
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Constructor. Aligned.
        \param length Length of the array.
        \param alignment Bytes alignment.
    */
    void init(u32 length, u32 alignment) {
        BaseArray::init(length, sizeof(T), alignment);
        mTStart = static_cast<T*>(mStart);
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param index Index (of the destiny array) from which to paste the other array.
    */
    void put(const Array<T>& other, u32 index){
        ASSERT(other.getLength() <= this->getLength() - index, "Not enough space for put array.");
        BaseArray::put(other.mStart, index, other.getLength()*mElementSize);
    };

    /*!
        \brief Copy an array into other.
        \param other Other Array.
        \param index Index (of the destiny array) from which to paste the other array.
        \param length Amount of element of the other array to be copied.
    */
    void put(const void* rawArray, u32 index, u32 length){
        ASSERT(length <= this->getLength() - index, "Not enough space for put array.");
        BaseArray::put(rawArray, index, length*mElementSize);
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t i) {
        ASSERT(i >= 0 && i < mLength, "Index out of bounds.");
        return mTStart[i];
    }

    /*!
        \brief Read only.
        \param i Index.
        \return Element reference.
    */
	T operator[](const size_t i) const {
		ASSERT(i >= 0 && i < mLength, "Index out of bounds.");
		return mTStart[i];
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
        (*this)[index] = element;
    };

};

} /* namespace DE */

#endif /* ARRAY_H_ */
