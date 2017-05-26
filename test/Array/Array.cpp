#include "Druid.h"

using namespace DE;

int main() {


	test(DE::Array<u32>);

	DE::LinearAllocator linear;
	linear.init(512);

	DE::Array<u32> array;

	u32 x[] = {1,2,3,4,5};
	array.init(x,5,&linear);

	expected_uint(array.getLength(),5);

	expected_uint(array[0],1);
	expected_uint(array[1],2);
	expected_uint(array[2],3);
	expected_uint(array[3],4);
	expected_uint(array[4],5);

	DE::Array<u32> copy(array);

	expected_uint(copy.getLength(),5);

	expected_uint(copy[0],1);
	expected_uint(copy[1],2);
	expected_uint(copy[2],3);
	expected_uint(copy[3],4);
	expected_uint(copy[4],5);

	u32 y[] = {6,7,8,9,10};
	array.init(y,5,&linear);

	expected_uint(array.getLength(),5);

	expected_uint(array[0],6);
	expected_uint(array[1],7);
	expected_uint(array[2],8);
	expected_uint(array[3],9);
	expected_uint(array[4],10);

	array.init(y,5,8,&linear);

	expected_uint(array.getLength(),5);

	expected_uint(array[0],6);
	expected_uint(array[1],7);
	expected_uint(array[2],8);
	expected_uint(array[3],9);
	expected_uint(array[4],10);

	DE::Array<u32> copy2(array);

	expected_uint(copy2.getLength(),5);

	expected_uint(copy2[0],6);
	expected_uint(copy2[1],7);
	expected_uint(copy2[2],8);
	expected_uint(copy2[3],9);
	expected_uint(copy2[4],10);

	linear.reset();

	summary();

	return 0;
}
