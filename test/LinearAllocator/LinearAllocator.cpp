#include "Druid.h"

int main() {

	test(DE::LinearAllocator);

	DE::LinearAllocator linear;
	linear.init(64);

	linear.init(1024);

	linear.init(50000);

	std::size_t sizeInt = sizeof(int);

	int* i = reinterpret_cast<int*>(linear.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_num(sizeInt,linear.getAllocated());

	int* j = reinterpret_cast<int*>(linear.allocateAligned(32,16));
	*j = 500;

	show(j);
	show(*j);

	expected_num(sizeInt+32+16,linear.getAllocated());

	linear.reset();

	expected_num(0,linear.getAllocated());

	summary();

	return 0;
}
