#include "Druid.h"

using namespace DE;

int main() {

	test(DE::Matrix4);

	DE::LinearAllocator linear;
	linear.init(2048);

	DE::Matrix4* m = DE::allocate<Matrix4>(linear);

	m->init(1);

	expected_uint(m->get(0,0),1);

	DE::Matrix4* m2 = DE::allocate<Matrix4>(linear);

	m2->init(*m);

	expected_uint(m2->get(0,0),1);

	linear.reset();

	summary();

	return 0;
}
