#include "Test.hpp"
#include "Memory.hpp"
#include "Matrix4.hpp"
#include "Array.hpp"

using namespace DE;

int main() {

	DE::Memory::init();

	DE_test(DE::Matrix4);

	DE::Matrix4 *m = DE::Memory::allocate<Matrix4>();

	m->init(1);

	DE_test_expected_uint(m->get(0, 0), 1);
	DE_test_expected_uint(m->get(0, 1), 1);
	DE_test_expected_uint(m->get(0, 2), 1);
	DE_test_expected_uint(m->get(0, 3), 1);
	DE_test_expected_uint(m->get(1, 0), 1);
	DE_test_expected_uint(m->get(1, 1), 1);
	DE_test_expected_uint(m->get(1, 2), 1);
	DE_test_expected_uint(m->get(1, 3), 1);
	DE_test_expected_uint(m->get(2, 0), 1);
	DE_test_expected_uint(m->get(2, 1), 1);
	DE_test_expected_uint(m->get(2, 2), 1);
	DE_test_expected_uint(m->get(2, 3), 1);
	DE_test_expected_uint(m->get(3, 0), 1);
	DE_test_expected_uint(m->get(3, 1), 1);
	DE_test_expected_uint(m->get(3, 2), 1);
	DE_test_expected_uint(m->get(3, 3), 1);

	const f32 row[4] = { 3, 3, 3, 3 };
	m->init(row, row, row, row);

	DE_test_expected_uint(m->get(0, 0), 3);
	DE_test_expected_uint(m->get(0, 1), 3);
	DE_test_expected_uint(m->get(0, 2), 3);
	DE_test_expected_uint(m->get(0, 3), 3);
	DE_test_expected_uint(m->get(1, 0), 3);
	DE_test_expected_uint(m->get(1, 1), 3);
	DE_test_expected_uint(m->get(1, 2), 3);
	DE_test_expected_uint(m->get(1, 3), 3);
	DE_test_expected_uint(m->get(2, 0), 3);
	DE_test_expected_uint(m->get(2, 1), 3);
	DE_test_expected_uint(m->get(2, 2), 3);
	DE_test_expected_uint(m->get(2, 3), 3);
	DE_test_expected_uint(m->get(3, 0), 3);
	DE_test_expected_uint(m->get(3, 1), 3);
	DE_test_expected_uint(m->get(3, 2), 3);
	DE_test_expected_uint(m->get(3, 3), 3);

	DE::Matrix4 *m2 = DE::Memory::allocate<Matrix4>();

	m2->init(*m);

	DE_test_expected_uint(m2->get(0, 0), 3);

	DE::Array<f32> *array = DE::Memory::allocate<Array<f32>>();
	array->init(16);
	array->fill(1);

	DE::Matrix4 *m3 = DE::Memory::allocate<Matrix4>();

	m3->init(*array);

	DE_test_expected_uint(m3->get(0, 0), 1);
	DE_test_expected_uint(m3->get(0, 1), 1);
	DE_test_expected_uint(m3->get(0, 2), 1);
	DE_test_expected_uint(m3->get(0, 3), 1);
	DE_test_expected_uint(m3->get(1, 0), 1);
	DE_test_expected_uint(m3->get(1, 1), 1);
	DE_test_expected_uint(m3->get(1, 2), 1);
	DE_test_expected_uint(m3->get(1, 3), 1);
	DE_test_expected_uint(m3->get(2, 0), 1);
	DE_test_expected_uint(m3->get(2, 1), 1);
	DE_test_expected_uint(m3->get(2, 2), 1);
	DE_test_expected_uint(m3->get(2, 3), 1);
	DE_test_expected_uint(m3->get(3, 0), 1);
	DE_test_expected_uint(m3->get(3, 1), 1);
	DE_test_expected_uint(m3->get(3, 2), 1);
	DE_test_expected_uint(m3->get(3, 3), 1);

	DE::Memory::free<DE::Matrix4>(m);
	DE::Memory::free<DE::Matrix4>(m2);
	DE::Memory::free<DE::Array<f32>>(array);
	DE::Memory::free<DE::Matrix4>(m3);

	DE::Memory::free();

	summary();

	return 0;
}
