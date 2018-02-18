#include "Druid.h"

using namespace DE;

int main() {


	test(DE::FreeListAllocator);

	DE::FreeListAllocator freeList;
	freeList.init(64);
	freeList.init(1024);

	u32 sizeInt = sizeof(u32);

	freeList.reset(); //TODO: FIX RESET

	u32* k = reinterpret_cast<u32*>(freeList.allocate(sizeInt,8));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(*k,300);

	expected_float(freeList.getAllocated(),sizeInt+8);

	u32* i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(*i,500);

	expected_float(freeList.getAllocated(),sizeInt+8+sizeInt+1); // +1 because always allocate a header for padding

	u32* j = reinterpret_cast<u32*>(freeList.allocate(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(*i,500);
	expected_uint(*j,700);

	expected_float(freeList.getAllocated(),sizeInt+8+sizeInt+1+sizeInt+16);

	freeList.reset(); //TODO: FIX RESET

	expected_float(freeList.getAllocated(),0);

	k = reinterpret_cast<u32*>(freeList.allocate(sizeInt,8));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(*k,300);

	expected_float(freeList.getAllocated(),sizeInt+8);

	i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(*i,500);

	expected_float(freeList.getAllocated(),sizeInt+8+sizeInt+1);

	j = reinterpret_cast<u32*>(freeList.allocate(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(*i,500);
	expected_uint(*j,700);

	expected_float(freeList.getAllocated(),sizeInt+8+sizeInt+1+sizeInt+16);

	summary();

	return 0;
}
