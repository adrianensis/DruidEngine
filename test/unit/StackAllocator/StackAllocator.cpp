#include "Druid.h"

using namespace DE;

int main() {


	test(DE::StackAllocator);

	DE::StackAllocator stack;
	// stack.init(64);
	stack.init(1024);
	// stack.init(50000);

	const u32 sizeInt = sizeof(u32);
	u32 headerSize = sizeof(u32);

	u32* a = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*a = 300;

	show(a);
	show(*a);

	expected_uint(*a,300);

	expected_uint(stack.getAllocated(),sizeInt+headerSize+1);

	stack.free();

	expected_uint(stack.getAllocated(),0);

	//------------------------------------------------------

	u32 total = 0;

	u32* b = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*b = 300;
	total+=sizeInt+headerSize+1;

	show(b);
	show(*b);

	expected_uint(*b,300);

	expected_uint(stack.getAllocated(),total);

	//------------------------------------------------------

	u32* c = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*c = 500;
	total+=sizeInt+headerSize+1;

	show(c);
	show(*c);

	expected_uint(*b,300);
	expected_uint(*c,500);

	expected_uint(stack.getAllocated(),total);

	//------------------------------------------------------

	u32*d = reinterpret_cast<u32*>(stack.allocate(sizeInt,16));
	*d = 700;

	total+=sizeInt+headerSize+16;

	show(d);
	show(*d);

	expected_uint(*b,300);
	expected_uint(*c,500);
	expected_uint(*d,700);

	expected_uint(stack.getAllocated(),total);

	//------------------------------------------------------

	show(stack.getAllocated());

	stack.free(); // pop d

	show(stack.getAllocated());

	stack.free(); // pop c

	show(stack.getAllocated());

	stack.free(); // pop b

	// stack.reset();

	expected_uint(stack.getAllocated(),0);

	stack.reset();

	expected_uint(stack.getAllocated(),0);

	summary();

	return 0;
}
