#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Memory/PoolAllocator.hpp"

using namespace DE;

int main() {

	test(DE::PoolAllocator);

	DE::PoolAllocator pool;

	u32 sizeInt = sizeof(u32);
	u32 sizePtr = sizeof(ptr);

	test_show(sizeInt);
	test_show(sizePtr);

	pool.init(sizeInt, 4);

	test_expected_uint(pool.getAllocatedSize(), (sizeInt + sizePtr + 1) * 4);
	test_expected_uint(pool.getFreeBlocks(), 4);

	u32* a = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 3);
	u32* b = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 2);
	u32* c = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 1);

	test_show(a);
	test_show(b);
	test_show(c);

	*a = 1;
	*b = 2;
	*c = 3;

	test_expected_uint(*a, 1);
	test_expected_uint(*b, 2);
	test_expected_uint(*c, 3);

	pool.free(reinterpret_cast<byte*>(b));

	test_expected_uint(pool.getFreeBlocks(), 2);

	u32* d = reinterpret_cast<u32*>(pool.allocateBlock());

	test_expected_uint(pool.getFreeBlocks(), 1);

	*d = 4;

	test_show(d);
	test_show(*d);

	test_show(a);
	test_show(c);

	test_expected_uint(*a, 1);
	test_expected_uint(*c, 3);

	pool.free(reinterpret_cast<byte*>(d));

	test_expected_uint(pool.getFreeBlocks(), 2);

	// ALIGNED

	pool.init(sizeInt, 4, 8);

	test_expected_uint(pool.getAllocatedSize(), (sizeInt + sizePtr + 8) * 4);
	test_expected_uint(pool.getFreeBlocks(), 4);

	a = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 3);
	b = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 2);
	c = reinterpret_cast<u32*>(pool.allocateBlock());
	test_expected_uint(pool.getFreeBlocks(), 1);

	test_show(a);
	test_show(b);
	test_show(c);

	*a = 1111;
	*b = 2222;
	*c = 3333;

	test_show(*a);
	test_show(*b);
	test_show(*c);

	pool.free(reinterpret_cast<byte*>(b));

	test_expected_uint(pool.getFreeBlocks(), 2);

	d = reinterpret_cast<u32*>(pool.allocateBlock());

	*d = 4444;

	test_show(d);
	test_show(*d);

	test_show(a);
	test_show(b);
	test_show(c);

	test_expected_uint(*a, 1111);
	test_expected_uint(*b, 2222);
	test_expected_uint(*c, 3333);

	pool.free(reinterpret_cast<byte*>(a));
	pool.free(reinterpret_cast<byte*>(c));
	pool.free(reinterpret_cast<byte*>(d));

	test_expected_uint(pool.getFreeBlocks(), 4);

	pool.allocateBlock();

	test_expected_uint(pool.getFreeBlocks(), 3);

	pool.reset();

	test_expected_uint(pool.getFreeBlocks(), 4);

	summary();

	return 0;
}
