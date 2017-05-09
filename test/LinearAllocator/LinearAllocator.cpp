#include "Druid.h"

using namespace DE;

int main() {


	test(DE::LinearAllocator);

	DE::LinearAllocator linear;
	linear.init(64);
	linear.init(1024);
	linear.init(50000);

	u32 sizeInt = sizeof(int);

	u32* i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_num(sizeInt,linear.getAllocated());

	u32* j = reinterpret_cast<u32*>(linear.allocateAligned(32,16));
	*j = 700;

	show(j);
	show(*j);

	expected_num(sizeInt+32+16,linear.getAllocated());

	linear.reset();

	expected_num(0,linear.getAllocated());

	summary();

	return 0;
}
