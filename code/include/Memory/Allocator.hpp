#ifndef DE_ALLOCATOR_H
#define DE_ALLOCATOR_H

#include "Assert.hpp"
#include "Basic.hpp"

namespace DE {

/*!
 \brief Manages memory allocation.
 */
class Allocator: public DE_Class {

protected:

	u32 mTotalSize;
	u32 mAllocatedSize;
	byte* mStart;

	void checkAllocate(u32 size) const;
	void checkAlignment(u32 alignment) const;
	void checkFree() const;

	byte* calculateAlignedAddress(const byte *unalignedAddress, u32 alignment) const;
	byte* calculateUnalignedAddress(const byte *alignedAddress) const;
	byte* allocateAlignedAddress(byte *unalignedAddress, u32 size, u32 alignment);

	void setAllocatedSize(u32 size);
	void clean(byte *mem, u32 size);
	void setMemoryChunk(byte *mem);

	// Debug

	u32 mDebugMaxAllocatedSize;

public:

	DE_CLASS(Allocator, DE_Class)

	u32 getTotalSize() const {
		return mTotalSize;
	};

	u32 getAllocatedSize() const {
		return mAllocatedSize;
	};

	u32 getDebugMaxAllocatedSize() const {
		return mDebugMaxAllocatedSize;
	};


	/*!
	 \return True if space is enough.
	 \param size Size you want to check.
	 */
	bool hasSpace(u32 size) const;

	/*!
	 \brief Constructor.
	 \param size Amount of memory you want to allocate.
	 */
	virtual void init(u32 size);

	/*!
	 \brief Constructor.
	 \param size Amount of memory you want to allocate.
	 \param mem Pointer to pre-allocated memory chunk.
	 */
	virtual void initFromMemory(u32 size, byte *mem);

	/*!
	 \brief Allocates memory.
	 \param size Amount of memory you want to allocate.
	 \return Pointer to memory chunk.
	 */
	virtual byte* allocate(u32 size) = 0;

	/*!
	 \brief Allocates memory.
	 \param size Amount of memory you want to allocate.
	 \param alignment Bytes alignment.
	 \return Pointer to memory chunk.
	 */
	virtual byte* allocate(u32 size, u32 alignment) = 0;

	/*!
	 \brief Frees memory.
	 \param pointer Pointer to memory.
	 */
	virtual void free(const byte *pointer) = 0;

	/*!
	 \brief Cleaning step. Used by FreeListAllocator to remove invalid blocks.
	 */
	virtual void flush();

	virtual void reset();

	/*!
	 \brief Frees aligned memory.
	 */
	virtual void terminate();

	/*!
	 \brief Constructs objects. It is used like "new" keyword.
	 Allocate an object of T class, using the allocator.

	 \tparam T Class.
	 \param allocator Allocator used to allocate memory.
	 \param alignment Bytes alignment.
	 */
	template<class T>
	static T* internalAllocate(Allocator *allocator, u32 alignment) {
		T* object = new (allocator->allocate(sizeof(T), alignment)) T;
		return object;
	};


	/*!
	 \brief Constructs objects. It is used like "new" keyword.
	 Allocate an object of T class, using the allocator.

	 \tparam T Class
	 \param allocator Allocator used to allocate memory.
	 */
	template<class T>
	static T* internalAllocate(Allocator *allocator) {
		T* object = new (allocator->allocate(sizeof(T))) T;
		return object;
	};


	/*!
	 \brief Destroys objects. It is used like "delete" keyword.
	 Deallocate data, using the allocator.

	 \tparam pointer pointer to data
	 \param allocator Allocator used to deallocate memory.
	 */
	template<class T>
	static void internalFree(T *object, Allocator *allocator) {
		if (object != nullptr) {
			object->dynamicDestructor();//~T();
			allocator->free(reinterpret_cast<byte*>(object));
		}
	};


};

} /* namespace DE */

#endif /* DE_ALLOCATOR_H */
