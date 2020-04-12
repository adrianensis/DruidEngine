#include "Test.h"
#include "Memory.h"
#include "FreeListAllocator.h"

using namespace DE;

int main(){


	DE_test(DE::FreeListAllocator);

	DE::FreeListAllocator freeList;
	freeList.init(64);
	freeList.init(1024);

	u32 sizeInt = sizeof(u32);

	freeList.reset(); //TODO: FIX RESET

	u32* k = reinterpret_cast<u32*>(freeList.allocate(sizeInt,8));
	*k = 300;

	DE_test_show(k);
	DE_test_show(*k);

	DE_test_expected_uint(*k,300);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8);

	freeList.free(k);

	DE_test_expected_float(freeList.getAllocatedSize(),0);

	k = reinterpret_cast<u32*>(freeList.allocate(sizeInt,8));
	*k = 300;

	DE_test_show(k);
	DE_test_show(*k);

	DE_test_expected_uint(*k,300);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8);

	u32* i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	DE_test_show(i);
	DE_test_show(*i);

	DE_test_expected_uint(*i,500);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8+sizeInt+1); // +1 because always allocate a header for padding

	u32* j = reinterpret_cast<u32*>(freeList.allocate(sizeInt,16));
	*j = 700;

	DE_test_show(j);
	DE_test_show(*j);

	DE_test_expected_uint(*i,500);
	DE_test_expected_uint(*j,700);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8+sizeInt+1+sizeInt+16);

	freeList.free(j);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8+sizeInt+1);

	freeList.reset(); //TODO: FIX RESET

	DE_test_expected_float(freeList.getAllocatedSize(),0);

	k = reinterpret_cast<u32*>(freeList.allocate(sizeInt,8));
	*k = 300;

	DE_test_show(k);
	DE_test_show(*k);

	DE_test_expected_uint(*k,300);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8);

	i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	DE_test_show(i);
	DE_test_show(*i);

	DE_test_expected_uint(*i,500);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8+sizeInt+1);

	j = reinterpret_cast<u32*>(freeList.allocate(sizeInt,16));
	*j = 700;

	DE_test_show(j);
	DE_test_show(*j);

	DE_test_expected_uint(*i,500);
	DE_test_expected_uint(*j,700);

	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+8+sizeInt+1+sizeInt+16);

	freeList.free(k);
	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+1+sizeInt+16);
	freeList.free(i);
	DE_test_expected_float(freeList.getAllocatedSize(),sizeInt+16);
	freeList.free(j);
	DE_test_expected_float(freeList.getAllocatedSize(),0);

	summary();

	return 0;
}
