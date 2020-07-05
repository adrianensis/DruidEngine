#ifndef DE_MEMORY_H
#define DE_MEMORY_H

#include "Allocator.hpp"

#define KB (1024.0f)
#define MB (KB*1024.0f)
#define GB (MB*1024.0f)

namespace DE {

class Allocator;
// class FreeListAllocator;
class LinearAllocator;

class Memory: public DE_Class {

private:

	/*
	 * Global Heap for general purposes
	 * Special Heap for gameObjects (spawn and destroy)
	 * Level-Loading Heap
	 * Stack-allocator for single-frame allocations (cleared every frame)
	 * Debug memory Heap, only for allocations that will not be needed in final game.
	 */

	// static FreeListAllocator smGlobal;
	static LinearAllocator smGlobal;

	// static DE::Array<bool>* mFlags;
	// static DE::Array<PoolAllocator>* mPools;

	//std::map <string, PoolAllocator> mPools;

	Memory();
	~Memory();

public:

	// static FreeListAllocator smGlobal;

	static void init();

	static void init(u32 size);

	static void flush();

	static Allocator& getGlobal();

	template<class T>
	static T* allocate(u32 alignment) {
		// ECHO("ALLOCATE " + T::getClassNameStatic() + " " + std::to_string(T::getClassIdStatic()))
		return Allocator::internalAllocate<T>((Allocator*) (&smGlobal), alignment);
	}

	template<class T>
	static T* allocate() {
		// ECHO("ALLOCATE " + T::getClassNameStatic() + " " + std::to_string(T::getClassIdStatic()))
		return Allocator::internalAllocate<T>((Allocator*) (&smGlobal));
	}

	template<class T>
	static void free(T *pointer) {
		if (pointer) {
			// ECHO("FREE " + pointer->getClassName() + " " + std::to_string(pointer->getClassId()))
			Allocator::internalFree<T>(pointer, (Allocator*) (&smGlobal));
		}
	}

	static void free();

};

} /* namespace DE */

#endif /* DE_MEMORY_H */
