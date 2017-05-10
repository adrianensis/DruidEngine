#include "Druid.h"

using namespace DE;

int main() {


	test(DE::LinearAllocator);

	DE::LinearAllocator linear;
	linear.init(64);
	linear.init(1024);
	linear.init(50000);

	u32 sizeInt = sizeof(u32);

	u32* k = reinterpret_cast<u32*>(linear.allocateAligned(sizeInt,8));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(300,*k);

	expected_float(sizeInt+8,linear.getAllocated());

	u32* i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(500,*i);

	expected_float(sizeInt+8+sizeInt,linear.getAllocated());

	u32* j = reinterpret_cast<u32*>(linear.allocateAligned(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(500,*i);
	expected_uint(700,*j);

	expected_float(sizeInt+8+sizeInt+sizeInt+16,linear.getAllocated());

	linear.reset();

	expected_float(0,linear.getAllocated());

	summary();

	return 0;
}
