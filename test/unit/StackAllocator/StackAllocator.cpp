#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Memory/StackAllocator.hpp"

using namespace DE;

int main() {

	test(DE::StackAllocator);

	DE::StackAllocator stack;
	// stack.init(64);
	stack.init(1024);
	// stack.init(50000);

	u32 sizeInt = sizeof(u32);
	u32 headerSize = sizeof(u32);

	u32* a = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*a = 300;

	test_show(a);
	test_show(*a);

	test_expected_uint(*a, 300);

	test_expected_uint(stack.getAllocatedSize(), sizeInt + headerSize + 1);

	stack.free();

	test_expected_uint(stack.getAllocatedSize(), 0);

	//------------------------------------------------------

	u32 total = 0;

	u32* b = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*b = 300;
	total += sizeInt + headerSize + 1;

	test_show(b);
	test_show(*b);

	test_expected_uint(*b, 300);

	test_expected_uint(stack.getAllocatedSize(), total);

	//------------------------------------------------------

	u32* c = reinterpret_cast<u32*>(stack.allocate(sizeInt));
	*c = 500;
	total += sizeInt + headerSize + 1;

	test_show(c);
	test_show(*c);

	test_expected_uint(*b, 300);
	test_expected_uint(*c, 500);

	test_expected_uint(stack.getAllocatedSize(), total);

	//------------------------------------------------------

	u32* d = reinterpret_cast<u32*>(stack.allocate(sizeInt, 16));
	*d = 700;

	total += sizeInt + headerSize + 16;

	test_show(d);
	test_show(*d);

	test_expected_uint(*b, 300);
	test_expected_uint(*c, 500);
	test_expected_uint(*d, 700);

	test_expected_uint(stack.getAllocatedSize(), total);

	//------------------------------------------------------

	test_show(stack.getAllocatedSize());

	stack.free(); // pop d

	test_show(stack.getAllocatedSize());

	stack.free(); // pop c

	test_show(stack.getAllocatedSize());

	stack.free(); // pop b

	// stack.reset();

	test_expected_uint(stack.getAllocatedSize(), 0);

	stack.reset();

	test_expected_uint(stack.getAllocatedSize(), 0);

	summary();

	return 0;
}
