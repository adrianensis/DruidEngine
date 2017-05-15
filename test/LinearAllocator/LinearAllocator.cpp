#include "Druid.h"

using namespace DE;

int main() {


	test(DE::LinearAllocator);

	DE::LinearAllocator linear;
	linear.init(64);
	linear.init(1024);

	u32 sizeInt = sizeof(u32);

	u32* k = reinterpret_cast<u32*>(linear.allocate(sizeInt,8));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(*k,300);

	expected_float(linear.getAllocated(),sizeInt+8);

	u32* i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(*i,500);

	expected_float(linear.getAllocated(),sizeInt+8+sizeInt);

	u32* j = reinterpret_cast<u32*>(linear.allocate(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(*i,500);
	expected_uint(*j,700);

	expected_float(linear.getAllocated(),sizeInt+8+sizeInt+sizeInt+16);

	linear.reset();

	expected_float(linear.getAllocated(),0);

	linear.setReverse(true);

	k = reinterpret_cast<u32*>(linear.allocate(sizeInt,8));
	*k = 300;

	show(k);
	show(*k);

	expected_uint(*k,300);

	expected_float(linear.getAllocated(),sizeInt+8);

	i = reinterpret_cast<u32*>(linear.allocate(sizeInt));
	*i = 500;

	show(i);
	show(*i);

	expected_uint(*i,500);

	expected_float(linear.getAllocated(),sizeInt+8+sizeInt);

	j = reinterpret_cast<u32*>(linear.allocate(sizeInt,16));
	*j = 700;

	show(j);
	show(*j);

	expected_uint(*i,500);
	expected_uint(*j,700);

	expected_float(linear.getAllocated(),sizeInt+8+sizeInt+sizeInt+16);

	summary();

	return 0;
}
