#ifndef ISEQUENTIALCONTAINER_H_
#define ISEQUENTIALCONTAINER_H_

#include "Basic.h"
#include "Allocator.h"
#include "BaseContainer.h"

namespace DE {

/*!
    \brief ISequentialContainer.
*/
template<class T>
class ISequentialContainer : virtual public BaseContainer {

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
};

} /* namespace DE */

#endif /* ISEQUENTIALCONTAINER_H_ */
