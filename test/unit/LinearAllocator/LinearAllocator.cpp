#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Memory/LinearAllocator.hpp"

using namespace DE;

int main() {

	DE_test(DE::LinearAllocator);

	DE::LinearAllocator linear;
	linear.init(64);
	linear.init(1024);

	u32 sizeInt = sizeof(u32);

	u32* k = reinterpret_cast<u32*>(linear.allocate(sizeInt, 8));
	*k = 300;

	DE_test_show(k);
	DE_test_show(*k);

	DE_test_expected_uint(*k, 300);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8);

	u32* i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	DE_test_show(i);
	DE_test_show(*i);

	DE_test_expected_uint(*i, 500);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8 + sizeInt + 1); // +1 because always allocate a header for padding

	u32* j = reinterpret_cast<u32*>(linear.allocate(sizeInt, 16));
	*j = 700;

	DE_test_show(j);
	DE_test_show(*j);

	DE_test_expected_uint(*i, 500);
	DE_test_expected_uint(*j, 700);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8 + sizeInt + 1 + sizeInt + 16);

	linear.reset();

	DE_test_expected_float(linear.getAllocatedSize(), 0);

	k = reinterpret_cast<u32*>(linear.allocate(sizeInt, 8));
	*k = 300;

	DE_test_show(k);
	DE_test_show(*k);

	DE_test_expected_uint(*k, 300);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8);

	i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	DE_test_show(i);
	DE_test_show(*i);

	DE_test_expected_uint(*i, 500);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8 + sizeInt + 1);

	j = reinterpret_cast<u32*>(linear.allocate(sizeInt, 16));
	*j = 700;

	DE_test_show(j);
	DE_test_show(*j);

	DE_test_expected_uint(*i, 500);
	DE_test_expected_uint(*j, 700);

	DE_test_expected_float(linear.getAllocatedSize(), sizeInt + 8 + sizeInt + 1 + sizeInt + 16);

	summary();

	return 0;
}
