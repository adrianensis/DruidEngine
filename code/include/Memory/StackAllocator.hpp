#ifndef DE_STACKALLOCATOR_H
#define DE_STACKALLOCATOR_H

#include "LinearAllocator.hpp"
#include "Basic.hpp"

namespace DE {

/*!
 \brief Allocates memory in FIFO strategy.
 */
class StackAllocator: public LinearAllocator {

private:

	byte *mTop;

	static const u32 smHeaderSize;
	void storeHeader(const byte *address, u32 size);

public:

	DE_CLASS(StackAllocator, LinearAllocator)

	/*!
	 \return Pointer to the top of the stack.
	 */
	byte* getTop();

	void init(u32 size) override;
	void initFromMemory(u32 size, byte *mem) override;
	byte* allocate(u32 size) override;
	byte* allocate(u32 size, u32 alignment) override;
	void free(const byte *pointer) override;

	/*!
	 \brief Free the top of the stack.
	 */
	void free();

	void reset() override;

};

} /* namespace DE */

#endif /* DE_STACKALLOCATOR_H */
