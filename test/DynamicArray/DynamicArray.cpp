#include "Druid.h"

using namespace DE;

int main() {


	test(DE::DynamicArray<u32>);

	DE::Memory::init();

	DE::DynamicArray<u32>* array = DE::Memory::allocate<DynamicArray<u32>>();

	array->init(5);
	u32 n = (*array)[0];

	expected_uint(array->getLength(),5);

	DE::DynamicArray<u32>* array0 = DE::Memory::allocate<DynamicArray<u32>>();

	u32 x0[] = {1,2,3,4,5,6,7,8,9,10};
	array0->init(x0,10);

	expected_uint(array0->getLength(),10);

	for (int i = 0; i < 10; ++i) {
		expected_uint((*array0)[i],i+1);
	}

	DE::DynamicArray<u32>* array1 = DE::Memory::allocate<DynamicArray<u32>>();
	u32 size = 200;
	array1->init(size);

	for (u32 i = 0; i < size; ++i) {
		(*array1)[i] = i;
	}

	expected_uint(array1->getLength(),size);

	for (int i = 0; i < size; ++i) {
		expected_uint((*array1)[i],i);
	}

	DE::DynamicArray<u32>* array2 = DE::Memory::allocate<DynamicArray<u32>>();
	size = 657;
	array2->init(size);

	for (u32 i = 0; i < size; ++i) {
		(*array2)[i] = i;
	}

	expected_uint(array2->getLength(),size);

	for (int i = 0; i < size; ++i) {
		expected_uint((*array2)[i],i);
	}

	DE::DynamicArray<u32>* array3 = DE::Memory::allocate<DynamicArray<u32>>();

	array3->init();

	expected_uint(array3->getLength(),0);

	DE::DynamicArray<u32>* copy = DE::Memory::allocate<DynamicArray<u32>>();
	copy->init(*array0);

	expected_uint(copy->getLength(),10);

	expected_uint((*copy)[0],1);
	expected_uint((*copy)[1],2);
	expected_uint((*copy)[2],3);
	expected_uint((*copy)[3],4);
	expected_uint((*copy)[4],5);

	DE::DynamicArray<u32>* array4 = DE::Memory::allocate<DynamicArray<u32>>();
	array4->init(10);

	DE::DynamicArray<u32>* array5 = DE::Memory::allocate<DynamicArray<u32>>();

	u32 x5[] = {1,2,3,4,5};
	array5->init(x5,5);

	array4->put(*array5, 0, 0);

	expected_uint(array5->getLength(),5);
	expected_uint(array4->getLength(),10);

	expected_uint(array4->get(0),1);
	expected_uint((*array4)[0],1);
	expected_uint((*array4)[1],2);
	expected_uint((*array4)[2],3);
	expected_uint((*array4)[3],4);
	expected_uint((*array4)[4],5);

	array4->put(*array0, 5, 0);

	expected_uint((*array4)[5],1);
	expected_uint((*array4)[6],2);
	expected_uint((*array4)[7],3);
	expected_uint((*array4)[8],4);
	expected_uint((*array4)[9],5);

	array4->clear();
	expected_uint(array4->getLength(),0);

	array5->clear();
	expected_uint(array5->getLength(),0);

	DE::Memory::free();

	summary();

	return 0;
}
