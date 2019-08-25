#include "Test.h"
#include "Memory.h"
#include "List.h"
#include "Array.h"

using namespace DE;

class Test {
public:
	int n;
	Test (){n = 3;};
};

int main() {

	DE::Memory::init();

	DE_test(DE::List<Test>);

	List<Array<u32>*>* mArrays = DE::Memory::allocate< List<Array<u32>*> >();
	Array<u32>* newArray = DE::Memory::allocate<Array<u32>>();

	newArray->init(100);
	mArrays->init();

	mArrays->pushBack(newArray);

	auto itArrays = mArrays->getIterator();
	DE_test_expected_uint(itArrays.get()->getLength(),100);

	DE::List<Test>* list = DE::Memory::allocate<List<Test>>();

	list->init();

	DE_test_expected_uint(list->getLength(),0);

	Test a;
	Test b;
	Test c;

	list->pushBack(a);
	list->pushBack(b);
	list->pushBack(c);

	DE_test_expected_uint(list->getLength(),3);

	Test cc = list->popBack();
	cc.n = 7;

	DE_test_expected_bool(cc.n == c.n, false);

	list->clear();

	DE_test_expected_uint(list->getLength(),0);

	a.n = 0;
	b.n = 1;
	c.n = 2;

	list->pushBack(a);
	list->pushBack(b);
	list->pushBack(c);

	auto it = list->getIterator();

	DE_test_expected_uint(it.get().n,0);
	DE_test_expected_bool(it.hasNext(),true);
	it.next();
	DE_test_expected_uint(it.get().n,1);
	DE_test_expected_bool(it.hasNext(),true);
	it.next();
	DE_test_expected_uint(it.get().n,2);
	DE_test_expected_bool(it.hasNext(),false);

	it = list->getIterator();

	DE_test_expected_uint(it.get().n,0);

	list->remove(it);

	DE_test_expected_uint(list->getLength(),2);

	it = list->getIterator();

	DE_test_expected_uint(it.get().n,1);
	DE_test_expected_bool(it.hasNext(),true);
	it.next();
	DE_test_expected_uint(it.get().n,2);
	DE_test_expected_bool(it.hasNext(),false);








	DE::List<u32>* list32 = DE::Memory::allocate<List<u32>>();

	list32->init();

	DE_test_expected_uint(list32->getLength(),0);

	u32 sizeInt = sizeof(u32);
	u32 a32;// = (u32*)linear.allocate(sizeInt);
	u32 b32;// = (u32*)linear.allocate(sizeInt);
	u32 c32;// = (u32*)linear.allocate(sizeInt);

	a32 = 1;
	b32 = 2;
	c32 = 3;

	list32->pushBack(a32);
	list32->pushBack(b32);
	list32->pushBack(c32);

	DE_test_expected_uint(list32->getLength(),3);

	List<u32>::Iterator it32 = list32->getIterator();

	DE_test_expected_uint((it32.get()),1);
	it32.next();
	DE_test_expected_uint((it32.get()),2);
	it32.next();
	DE_test_expected_uint((it32.get()),3);

	DE_test_expected_uint(list32->getLength(),3);

	DE::Memory::free<List<Array<u32>*>>(mArrays);
	DE::Memory::free<Array<u32>>(newArray);
	DE::Memory::free<DE::List<Test>>(list);
	DE::Memory::free<DE::List<u32>>(list32);

	DE::Memory::free();

	summary();

	return 0;
}
