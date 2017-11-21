#include "Druid.h"

using namespace DE;

int main() {

	test(DE::Matrix4);

	DE::LinearAllocator linear;
	linear.init(1024);

	linear.reset();

	expected_uint(1,1);

	summary();

	return 0;
}
