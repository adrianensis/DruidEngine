#ifndef IARRAY_H_
#define IARRAY_H_

#include "ISequentialContainer.h"

namespace DE {

/*!
    \brief IArray.
*/
template <class T>
class IArray : virtual public ISequentialContainer<T> {

public:

    /*!
        \brief Default Constructor.
    */
    IArray() : ISequentialContainer<T>(){

    };

    /*!
        \brief Destructor.
    */
    ~IArray() override = default;

    /*!
        \brief Constructor.
        \param length Length of the array.
    */
    virtual void init(const u32 length) = 0;

    /*!
        \brief Constructor. Aligned.
        \param length Length of the array.
        \param alignment Bytes alignment.
    */
    virtual void init(const u32 length, const u32 alignment) = 0;


};

} /* namespace DE */

#endif /* IARRAY_H_ */
