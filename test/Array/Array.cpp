#include "Druid.h"

using namespace DE;

int main() {


	test(DE::Array<u32>);

	DE::LinearAllocator linear;
	linear.init(512);

	DE::Array<u32> array;

	u32 x[] = {1,2,3,4,5};
	array.init(x,5, &linear);

	expected_uint(array.getLength(),5);

	expected_uint(array[0],1);
	expected_uint(array[1],2);
	expected_uint(array[2],3);
	expected_uint(array[3],4);
	expected_uint(array[4],5);

	summary();

	return 0;
}
