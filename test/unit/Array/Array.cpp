#include "Test.hpp"
#include "Memory.hpp"
#include "Array.hpp"

using namespace DE;

int main(){

	DE::Memory::init();

	DE_test(DE::Array<u32>);


	DE::Array<u32>* array = DE::Memory::allocate<Array<u32>>();

	u32 x[] = {1,2,3,4,5};
	array->init(x,5);

	DE_test_expected_uint(array->getLength(),5);

	DE_test_expected_uint((*array)[0],1);
	DE_test_expected_uint((*array)[1],2);
	DE_test_expected_uint((*array)[2],3);
	DE_test_expected_uint((*array)[3],4);
	DE_test_expected_uint((*array)[4],5);

	DE::Array<u32>* copy = DE::Memory::allocate<Array<u32>>();
	copy->init(*array);

	DE_test_expected_uint(copy->getLength(),5);

	DE_test_expected_uint((*copy)[0],1);
	DE_test_expected_uint((*copy)[1],2);
	DE_test_expected_uint((*copy)[2],3);
	DE_test_expected_uint((*copy)[3],4);
	DE_test_expected_uint((*copy)[4],5);

	u32 y[] = {6,7,8,9,10};
	array->init(y,5);

	DE_test_expected_uint(array->getLength(),5);

	DE_test_expected_uint((*array)[0],6);
	DE_test_expected_uint((*array)[1],7);
	DE_test_expected_uint((*array)[2],8);
	DE_test_expected_uint((*array)[3],9);
	DE_test_expected_uint((*array)[4],10);

	array->init(y,5,8);

	DE_test_expected_uint(array->getLength(),5);

	DE_test_expected_uint((*array)[0],6);
	DE_test_expected_uint((*array)[1],7);
	DE_test_expected_uint((*array)[2],8);
	DE_test_expected_uint((*array)[3],9);
	DE_test_expected_uint((*array)[4],10);

	DE::Array<u32>* copy2 = DE::Memory::allocate<Array<u32>>();
	copy2->init(*array);

	DE_test_expected_uint(copy2->getLength(),5);

	DE_test_expected_uint((*copy2)[0],6);
	DE_test_expected_uint((*copy2)[1],7);
	DE_test_expected_uint((*copy2)[2],8);
	DE_test_expected_uint((*copy2)[3],9);
	DE_test_expected_uint((*copy2)[4],10);

	DE::Array<u32>* array2 = DE::Memory::allocate<Array<u32>>();
	array2->init(10);

	array2->put(*array, 0, 0);

	DE_test_expected_uint(array2->getLength(),10);

	DE_test_expected_uint((*array2)[0],6);
	DE_test_expected_uint((*array2)[1],7);
	DE_test_expected_uint((*array2)[2],8);
	DE_test_expected_uint((*array2)[3],9);
	DE_test_expected_uint((*array2)[4],10);

	array2->put(*array, 5, 0);

	DE_test_expected_uint((*array2)[5],6);
	DE_test_expected_uint((*array2)[6],7);
	DE_test_expected_uint((*array2)[7],8);
	DE_test_expected_uint((*array2)[8],9);
	DE_test_expected_uint((*array2)[9],10);

	array2->clear();

	DE_test_expected_uint(array2->getLength(),0);

	DE::Memory::free<Array<u32>>(array);
	DE::Memory::free<Array<u32>>(copy);
	DE::Memory::free<Array<u32>>(copy2);
	DE::Memory::free<Array<u32>>(array2);

	DE::Memory::free();

	summary();

	return 0;
}
