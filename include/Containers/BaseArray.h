#ifndef BASEARRAY_H_
#define BASEARRAY_H_

#include "Container.h"
#include "Basic.h"
#include "Allocator.h"

namespace DE {

DE_CLASS(BaseArray) DE_CLASS_EXTENDS public Container {

protected:

    void* mStart;

    void copy(const BaseArray& other);
    void init(const void* rawArray, const u32 length, const u32 elementSize);
    void init(const void* rawArray, const u32 length, const u32 elementSize, const u32 alignment);
    void init(const u32 length, const u32 elementSize);
    void init(const u32 length, const u32 elementSize, const u32 alignment);
    void allocate(const u32 length, const u32 elementSize, const u32 alignment);
    void set(const void* rawArray);

public:

    BaseArray();
    virtual ~BaseArray();

    void put(const void* rawArray, const u32 destinyIndex, const u32 sourceIndex, const u32 length);

};

} /* namespace DE */

#endif /* BASEARRAY_H_ */
