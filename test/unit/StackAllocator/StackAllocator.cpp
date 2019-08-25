#include "Test.h"
#include "Memory.h"
#include "StackAllocator.h"

using namespace DE;

int main() {


	DE_test(DE::StackAllocator);

	DE::StackAllocator stack;
	// stack.init(64);
	stack.init(1024);
	// stack.init(50000);

	u32 sizeInt = sizeof(u32);
	u32 headerSize = sizeof(u32);

	u32* a = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*a = 300;

	DE_test_show(a);
	DE_test_show(*a);

	DE_test_expected_uint(*a,300);

	DE_test_expected_uint(stack.getAllocatedSize(),sizeInt+headerSize+1);

	stack.free();

	DE_test_expected_uint(stack.getAllocatedSize(),0);

	//------------------------------------------------------

	u32 total = 0;

	u32* b = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*b = 300;
	total+=sizeInt+headerSize+1;

	DE_test_show(b);
	DE_test_show(*b);

	DE_test_expected_uint(*b,300);

	DE_test_expected_uint(stack.getAllocatedSize(),total);

	//------------------------------------------------------

	u32* c = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*c = 500;
	total+=sizeInt+headerSize+1;

	DE_test_show(c);
	DE_test_show(*c);

	DE_test_expected_uint(*b,300);
	DE_test_expected_uint(*c,500);

	DE_test_expected_uint(stack.getAllocatedSize(),total);

	//------------------------------------------------------

	u32*d = reinterpret_cast<u32*>(stack.allocate(sizeInt,16));
	*d = 700;

	total+=sizeInt+headerSize+16;

	DE_test_show(d);
	DE_test_show(*d);

	DE_test_expected_uint(*b,300);
	DE_test_expected_uint(*c,500);
	DE_test_expected_uint(*d,700);

	DE_test_expected_uint(stack.getAllocatedSize(),total);

	//------------------------------------------------------

	DE_test_show(stack.getAllocatedSize());

	stack.free(); // pop d

	DE_test_show(stack.getAllocatedSize());

	stack.free(); // pop c

	DE_test_show(stack.getAllocatedSize());

	stack.free(); // pop b

	// stack.reset();

	DE_test_expected_uint(stack.getAllocatedSize(),0);

	stack.reset();

	DE_test_expected_uint(stack.getAllocatedSize(),0);

	summary();

	return 0;
}
