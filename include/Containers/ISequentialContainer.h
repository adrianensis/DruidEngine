#ifndef ISEQUENTIALCONTAINER_H_
#define ISEQUENTIALCONTAINER_H_

#include "Basic.h"
#include "IAllocator.h"
#include "BaseContainer.h"

namespace DE {

/*!
    \brief ISequentialContainer.
*/
template<class T>
class ISequentialContainer : virtual public BaseContainer {

protected:

    /*!
        \param index The index.
        \return Reference to Element at index.
    */
    virtual T& _get(const u32 index) const = 0;

    /*!
        \brief Check boundaries on put method.
    */
    virtual void checkPut(const ISequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) = 0;

public:

    /*!
        \brief Default Constructor.
    */
    ISequentialContainer() : BaseContainer(){

    };

    /*!
        \brief Destructor.
    */
    ~ISequentialContainer() override = default;

    /*!
        \param index The index.
        \return Element at index.
    */
    virtual T get(const u32 index) const = 0;

    /*!
        \brief Sets element at index.
        \param index The index.
        \param element The element.
    */
    virtual void set(const u32 index, const T element) = 0;

    /*!
        \brief Constructor from raw array.
        \param rawArray The raw array.
        \param length The length of the raw array.
    */
    virtual void init(const void* rawArray, const u32 length) = 0;

    /*!
        \brief Constructor from raw array. Aligned.
        \param rawArray The raw array.
        \param length The length of the raw array.
        \param alignment Bytes alignment.
    */
    virtual void init(const void* rawArray, const u32 length, const u32 alignment) = 0;

    /*!
        \brief Fill the seq. container with the same element.
        \param element The element.
    */
    virtual void fill(const T element) = 0;

    /*!
        \brief Copy an array into other.
        \param other Other IArray.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    void put(const ISequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex) {
        this->put(other, destinyIndex, sourceIndex, other.getLength());
    };

    /*!
        \brief Copy an array into other.
        \param other Other IArray.
        \param destinyIndex Index (of the destiny array) from which to paste the other array.
        \param sourceIndex Index (of the source array) from which to copy.
    */
    virtual void put(const ISequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) {
      this->checkPut(other, destinyIndex, sourceIndex, length);

      for (u32 i = 0; i < length; ++i)
          (*this)[destinyIndex + i] = other[sourceIndex + i];
    };

    /*!
        \brief Can be used for assignment.
        \param i Index.
        \return Element reference.
    */
    T& operator[](const size_t index) {
        return this->_get(index);
    };

    /*!
        \brief Read only.
        \param i Index.
        \return Element reference.
    */
  	T operator[](const size_t index) const {
          return this->_get(index);
  	}
};

} /* namespace DE */

#endif /* ISEQUENTIALCONTAINER_H_ */
