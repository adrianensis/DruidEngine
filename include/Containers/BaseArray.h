#ifndef BASEARRAY_H_
#define BASEARRAY_H_

#include "Container.h"
#include "BasicTypes.h"
#include "Allocator.h"

namespace DE {

class BaseArray : public Container {

protected:

    void* mStart;

    void copy(const BaseArray& other);
    void init(const void* rawArray, u32 length, u32 elementSize);
    void init(const void* rawArray, u32 length, u32 elementSize, u32 alignment);
    void init(u32 length, u32 elementSize);
    void init(u32 length, u32 elementSize, u32 alignment);
    void allocate(u32 length, u32 elementSize, u32 alignment);
    void set(const void* rawArray);

public:

    BaseArray();
    virtual ~BaseArray();

    void put(const void* rawArray, u32 destinyIndex, u32 sourceIndex, u32 length);

};

} /* namespace DE */

#endif /* BASEARRAY_H_ */
