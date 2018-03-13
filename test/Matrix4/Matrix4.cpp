#include "Druid.h"

using namespace DE;

int main() {

	test(DE::Matrix4);

	DE::Memory::init();

	DE::Matrix4* m = DE::Memory::allocate<Matrix4>();

	m->init(1);

	expected_uint(m->get(0,0),1);

	DE::Matrix4* m2 = DE::Memory::allocate<Matrix4>();

	m2->init(*m);

	expected_uint(m2->get(0,0),1);

	DE::Array<f32>* array = DE::Memory::allocate<Array<f32>>();
	array->init(16);
	array->fill(1);

	DE::Matrix4* m3 = DE::Memory::allocate<Matrix4>();

	m3->init(*array);

	expected_uint(m3->get(0,0),1);

	DE::Memory::free();

	summary();

	return 0;
}
