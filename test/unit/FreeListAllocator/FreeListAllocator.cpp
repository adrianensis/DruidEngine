#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Memory/FreeListAllocator.hpp"

using namespace DE;

int main() {

	test(DE::FreeListAllocator);

	DE::FreeListAllocator freeList;
	freeList.init(64);
	freeList.init(1024);

	u32 sizeInt = sizeof(u32);

	freeList.reset(); //TODO: FIX RESET

	u32* k = reinterpret_cast<u32*>(freeList.allocate(sizeInt, 8));
	*k = 300;

	test_show(k);
	test_show(*k);

	test_expected_uint(*k, 300);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8);

	freeList.free(reinterpret_cast<byte*>(k));

	test_expected_float(freeList.getAllocatedSize(), 0);

	k = reinterpret_cast<u32*>(freeList.allocate(sizeInt, 8));
	*k = 300;

	test_show(k);
	test_show(*k);

	test_expected_uint(*k, 300);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8);

	u32* i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	test_show(i);
	test_show(*i);

	test_expected_uint(*i, 500);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8 + sizeInt + 1); // +1 because always allocate a header for padding

	u32* j = reinterpret_cast<u32*>(freeList.allocate(sizeInt, 16));
	*j = 700;

	test_show(j);
	test_show(*j);

	test_expected_uint(*i, 500);
	test_expected_uint(*j, 700);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8 + sizeInt + 1 + sizeInt + 16);

	freeList.free(reinterpret_cast<byte*>(j));

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8 + sizeInt + 1);

	freeList.reset(); //TODO: FIX RESET

	test_expected_float(freeList.getAllocatedSize(), 0);

	k = reinterpret_cast<u32*>(freeList.allocate(sizeInt, 8));
	*k = 300;

	test_show(k);
	test_show(*k);

	test_expected_uint(*k, 300);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8);

	i = reinterpret_cast<u32*>(freeList.allocate(sizeInt));
	*i = 500;

	test_show(i);
	test_show(*i);

	test_expected_uint(*i, 500);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8 + sizeInt + 1);

	j = reinterpret_cast<u32*>(freeList.allocate(sizeInt, 16));
	*j = 700;

	test_show(j);
	test_show(*j);

	test_expected_uint(*i, 500);
	test_expected_uint(*j, 700);

	test_expected_float(freeList.getAllocatedSize(), sizeInt + 8 + sizeInt + 1 + sizeInt + 16);

	freeList.free(reinterpret_cast<byte*>(k));
	test_expected_float(freeList.getAllocatedSize(), sizeInt + 1 + sizeInt + 16);
	freeList.free(reinterpret_cast<byte*>(i));
	test_expected_float(freeList.getAllocatedSize(), sizeInt + 16);
	freeList.free(reinterpret_cast<byte*>(j));
	test_expected_float(freeList.getAllocatedSize(), 0);

	summary();

	return 0;
}
