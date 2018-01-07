#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "BasicTypes.h"
#include "Allocator.h"

namespace DE {

/*!
    \brief Generic container.
*/
class Container{

protected:

    u32 mLength;
    u32 mElementSize;
    u32 mAlignment;
    Allocator* mAllocator;

    void init(u32 length, u32 elementSize, u32 alignment);

public:

    /*!
        \brief Default Constructor.
    */
    Container();

    /*!
        \brief Destructor.
    */
    virtual ~Container();

    /*!
        \return Container length.
    */
    u32 getLength() const;

    /*!
        \return Container element size.
    */
    u32 getElementSize() const;

    /*!
        \return Container alignment.
    */
    u32 getAlignment() const;

    /*!
        \brief Sets the allocator.
        \param allocator Pointer to Allocator.
    */
    void setAllocator(Allocator* allocator);
};

} /* namespace DE */

#endif /* CONTAINER_H_ */
