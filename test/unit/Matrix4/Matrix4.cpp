#include "Test/Test.hpp"
#include "Maths/Matrix4.hpp"
#include <array>

int main(){

	test(Matrix4);

	Matrix4* m = new Matrix4();

	m->init(1);

	test_expected_uint(m->get(0, 0), 1);
	test_expected_uint(m->get(0, 1), 1);
	test_expected_uint(m->get(0, 2), 1);
	test_expected_uint(m->get(0, 3), 1);
	test_expected_uint(m->get(1, 0), 1);
	test_expected_uint(m->get(1, 1), 1);
	test_expected_uint(m->get(1, 2), 1);
	test_expected_uint(m->get(1, 3), 1);
	test_expected_uint(m->get(2, 0), 1);
	test_expected_uint(m->get(2, 1), 1);
	test_expected_uint(m->get(2, 2), 1);
	test_expected_uint(m->get(2, 3), 1);
	test_expected_uint(m->get(3, 0), 1);
	test_expected_uint(m->get(3, 1), 1);
	test_expected_uint(m->get(3, 2), 1);
	test_expected_uint(m->get(3, 3), 1);

	const f32 row[4] = { 3, 3, 3, 3 };
	m->init(row, row, row, row);

	test_expected_uint(m->get(0, 0), 3);
	test_expected_uint(m->get(0, 1), 3);
	test_expected_uint(m->get(0, 2), 3);
	test_expected_uint(m->get(0, 3), 3);
	test_expected_uint(m->get(1, 0), 3);
	test_expected_uint(m->get(1, 1), 3);
	test_expected_uint(m->get(1, 2), 3);
	test_expected_uint(m->get(1, 3), 3);
	test_expected_uint(m->get(2, 0), 3);
	test_expected_uint(m->get(2, 1), 3);
	test_expected_uint(m->get(2, 2), 3);
	test_expected_uint(m->get(2, 3), 3);
	test_expected_uint(m->get(3, 0), 3);
	test_expected_uint(m->get(3, 1), 3);
	test_expected_uint(m->get(3, 2), 3);
	test_expected_uint(m->get(3, 3), 3);

	Matrix4* m2 = new Matrix4();

	m2->init(*m);

	test_expected_uint(m2->get(0, 0), 3);

	std::array<f32, 16> array;
	array.fill(1);

	Matrix4* m3 = new Matrix4();

	m3->init(array);

	test_expected_uint(m3->get(0, 0), 1);
	test_expected_uint(m3->get(0, 1), 1);
	test_expected_uint(m3->get(0, 2), 1);
	test_expected_uint(m3->get(0, 3), 1);
	test_expected_uint(m3->get(1, 0), 1);
	test_expected_uint(m3->get(1, 1), 1);
	test_expected_uint(m3->get(1, 2), 1);
	test_expected_uint(m3->get(1, 3), 1);
	test_expected_uint(m3->get(2, 0), 1);
	test_expected_uint(m3->get(2, 1), 1);
	test_expected_uint(m3->get(2, 2), 1);
	test_expected_uint(m3->get(2, 3), 1);
	test_expected_uint(m3->get(3, 0), 1);
	test_expected_uint(m3->get(3, 1), 1);
	test_expected_uint(m3->get(3, 2), 1);
	test_expected_uint(m3->get(3, 3), 1);

	delete m;
	delete m2;
	delete m3;

	summary();

	return 0;
}
