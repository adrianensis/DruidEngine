#include "Druid.h"

using namespace DE;

class Test {
public:
	int n;
	Test (){n = 3;};
};

int main() {


	test(DE::List<Test>);

	DE::LinearAllocator linear;
	linear.init(1024);

	DE::List<Test>* list = DE::allocate<List<Test>>(linear);

	list->init();

	expected_uint(list->getLength(),0);

	Test a;
	Test b;
	Test c;

	list->pushBack(a);
	list->pushBack(b);
	list->pushBack(c);

	expected_uint(list->getLength(),3);

	Test cc = list->popBack();
	cc.n = 7;

	expected_bool(cc.n == c.n, false);

	list->clear();

	expected_uint(list->getLength(),0);

	linear.reset();

	summary();

	return 0;
}
