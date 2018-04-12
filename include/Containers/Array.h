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

    T& _get(const u32 index) const{
      DE_ASSERT(index >= 0 && index < ISequentialContainer<T>::getLength(), "Index out of bounds.");
      return mTStart[index];
    };

    void checkPut(const ISequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) override {
      DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
      DE_ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
      DE_ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
      DE_ASSERT(length <= this->getLength() - destinyIndex, "Not enough space to put array.");
    };

public:

    using IArray<T>::put; // because "put" method is ambiguous.

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

    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex) {
        Array::put(other, destinyIndex, sourceIndex, other.getLength());
    };

    void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) {
        this->checkPut(other, destinyIndex, sourceIndex, length);
        BaseArray::put(other.mStart, destinyIndex, sourceIndex, length);
    };

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
