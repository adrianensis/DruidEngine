#include "Druid.h"

using namespace DE;

int main() {


	test(DE::StackAllocator);

	DE::StackAllocator stack;
	stack.init(64);
	stack.init(1024);
	stack.init(50000);

	u32 sizeInt = sizeof(u32);
	u32 headerSize = sizeof(u32);

	u32* k = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(300,*k);

	expected_uint(sizeInt+headerSize,stack.getAllocated());

	stack.free();

	show(stack.getAllocated());

	expected_uint(0,stack.getAllocated());

	u32* i = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(500,*i);

	expected_uint(sizeInt+headerSize,stack.getAllocated());

	u32* j = reinterpret_cast<u32*>(stack.allocateAligned(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(500,*i);
	expected_uint(700,*j);

	// first allocation + second allocation
	expected_uint(sizeInt+headerSize+sizeInt+headerSize+16,stack.getAllocated());

	stack.freeAligned();
	stack.free();

	stack.reset();

	expected_uint(0,stack.getAllocated());

	summary();

	return 0;
}
