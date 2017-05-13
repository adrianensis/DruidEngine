#include "Druid.h"

using namespace DE;

int main() {


	test(DE::PoolAllocator);

	DE::PoolAllocator pool;

	u32 sizeInt = sizeof(u32);
	u32 sizePtr = sizeof(ptr);

	pool.init(sizeInt, 4, 0);

	expected_uint(pool.getAllocated(),0);

	u32* a = reinterpret_cast<u32*>(pool.allocate());
	u32* b = reinterpret_cast<u32*>(pool.allocate());
	u32* c = reinterpret_cast<u32*>(pool.allocate());

	show(a);
	show(b);
	show(c);

	*a = 1;
	*b = 2;
	*c = 3;

	expected_uint(*a,1);
	expected_uint(*b,2);
	expected_uint(*c,3);

	expected_uint(pool.getAllocated(),(sizeInt+sizePtr)*3);

	expected_uint(pool.getFreeBlocks(),1);

	pool.free(b);

	expected_uint(pool.getFreeBlocks(),2);

	expected_uint(pool.getAllocated(),(sizeInt+sizePtr)*2);

	u32* d = reinterpret_cast<u32*>(pool.allocate());

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

	expected_uint(pool.getAllocated(),0);

	a = reinterpret_cast<u32*>(pool.allocate());
	b = reinterpret_cast<u32*>(pool.allocate());
	c = reinterpret_cast<u32*>(pool.allocate());

	show(a);
	show(b);
	show(c);

	*a = 1;
	*b = 2;
	*c = 3;

	show(*a);
	show(*b);
	show(*c);

	expected_uint(pool.getAllocated(),8 + (sizeInt+sizePtr)*3);

	expected_uint(pool.getFreeBlocks(),1);

	pool.free(b);

	expected_uint(pool.getFreeBlocks(),2);

	expected_uint(pool.getAllocated(),8 + (sizeInt+sizePtr)*2);

	d = reinterpret_cast<u32*>(pool.allocate());

	*d = 4;

	show(d);
	show(*d);

	show(a);
	show(b);
	show(c);

	expected_uint(*a,1);
	expected_uint(*b,2);
	expected_uint(*c,3);

	pool.free(a);
	pool.free(c);
	pool.free(d);

	expected_uint(pool.getFreeBlocks(),4);

	pool.allocate();

	summary();

	return 0;
}
