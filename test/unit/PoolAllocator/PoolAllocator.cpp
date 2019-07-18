#include "Druid.h"

using namespace DE;

int main() {


	DE_test(DE::PoolAllocator);

	DE::PoolAllocator pool;

	u32 sizeInt = sizeof(u32);
	u32 sizePtr = sizeof(ptr);

	DE_test_show(sizeInt);
	DE_test_show(sizePtr);

	pool.init(sizeInt, 4);

	DE_test_expected_uint(pool.getAllocatedSize(),(sizeInt+sizePtr+1)*4);
	DE_test_expected_uint(pool.getFreeBlocks(),4);

	u32* a = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),3);
	u32* b = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),2);
	u32* c = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),1);

	DE_test_show(a);
	DE_test_show(b);
	DE_test_show(c);

	*a = 1;
	*b = 2;
	*c = 3;

	DE_test_expected_uint(*a,1);
	DE_test_expected_uint(*b,2);
	DE_test_expected_uint(*c,3);

	pool.free(b);

	DE_test_expected_uint(pool.getFreeBlocks(),2);

	u32* d = reinterpret_cast<u32*>(pool.allocateBlock());

	DE_test_expected_uint(pool.getFreeBlocks(),1);

	*d = 4;

	DE_test_show(d);
	DE_test_show(*d);

	DE_test_show(a);
	DE_test_show(c);

	DE_test_expected_uint(*a,1);
	DE_test_expected_uint(*c,3);

	pool.free(d);

	DE_test_expected_uint(pool.getFreeBlocks(),2);

	// ALIGNED

	pool.init(sizeInt, 4, 8);

	DE_test_expected_uint(pool.getAllocatedSize(),(sizeInt+sizePtr+8)*4);
	DE_test_expected_uint(pool.getFreeBlocks(),4);

	a = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),3);
	b = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),2);
	c = reinterpret_cast<u32*>(pool.allocateBlock());
	DE_test_expected_uint(pool.getFreeBlocks(),1);

	DE_test_show(a);
	DE_test_show(b);
	DE_test_show(c);

	*a = 1111;
	*b = 2222;
	*c = 3333;

	DE_test_show(*a);
	DE_test_show(*b);
	DE_test_show(*c);

	pool.free(b);

	DE_test_expected_uint(pool.getFreeBlocks(),2);

	d = reinterpret_cast<u32*>(pool.allocateBlock());

	*d = 4444;

	DE_test_show(d);
	DE_test_show(*d);

	DE_test_show(a);
	DE_test_show(b);
	DE_test_show(c);

	DE_test_expected_uint(*a,1111);
	DE_test_expected_uint(*b,2222);
	DE_test_expected_uint(*c,3333);

	pool.free(a);
	pool.free(c);
	pool.free(d);

	DE_test_expected_uint(pool.getFreeBlocks(),4);

	pool.allocateBlock();

	DE_test_expected_uint(pool.getFreeBlocks(),3);

	pool.reset();

	DE_test_expected_uint(pool.getFreeBlocks(),4);

	summary();

	return 0;
}
