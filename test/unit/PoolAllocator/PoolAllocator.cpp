#include "Druid.h"

using namespace DE;

int main() {


	test(DE::PoolAllocator);

	DE::PoolAllocator pool;

	u32 sizeInt = sizeof(u32);
	u32 sizePtr = sizeof(ptr);

	show(sizeInt);
	show(sizePtr);

	pool.init(sizeInt, 4);

	expected_uint(pool.getAllocated(),(sizeInt+sizePtr+1)*4);
	expected_uint(pool.getFreeBlocks(),4);

	u32* a = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),3);
	u32* b = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),2);
	u32* c = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),1);

	show(a);
	show(b);
	show(c);

	*a = 1;
	*b = 2;
	*c = 3;

	expected_uint(*a,1);
	expected_uint(*b,2);
	expected_uint(*c,3);

	pool.free(b);

	expected_uint(pool.getFreeBlocks(),2);

	u32* d = reinterpret_cast<u32*>(pool.allocateBlock());

	expected_uint(pool.getFreeBlocks(),1);

	*d = 4;

	show(d);
	show(*d);

	show(a);
	show(c);

	expected_uint(*a,1);
	expected_uint(*c,3);

	pool.free(d);

	expected_uint(pool.getFreeBlocks(),2);

	// ALIGNED

	pool.init(sizeInt, 4, 8);

	expected_uint(pool.getAllocated(),(sizeInt+sizePtr+8)*4);
	expected_uint(pool.getFreeBlocks(),4);

	a = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),3);
	b = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),2);
	c = reinterpret_cast<u32*>(pool.allocateBlock());
	expected_uint(pool.getFreeBlocks(),1);

	show(a);
	show(b);
	show(c);

	*a = 1111;
	*b = 2222;
	*c = 3333;

	show(*a);
	show(*b);
	show(*c);

	pool.free(b);

	expected_uint(pool.getFreeBlocks(),2);

	d = reinterpret_cast<u32*>(pool.allocateBlock());

	*d = 4444;

	show(d);
	show(*d);

	show(a);
	show(b);
	show(c);

	expected_uint(*a,1111);
	expected_uint(*b,2222);
	expected_uint(*c,3333);

	pool.free(a);
	pool.free(c);
	pool.free(d);

	expected_uint(pool.getFreeBlocks(),4);

	pool.allocateBlock();

	expected_uint(pool.getFreeBlocks(),3);

	pool.reset();

	expected_uint(pool.getFreeBlocks(),4);

	summary();

	return 0;
}
