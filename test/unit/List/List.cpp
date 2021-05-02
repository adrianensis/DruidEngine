#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Containers/List.hpp"

using namespace DE;

int main() {

	DE::Memory::init();

	test(DE::List<u32>);

	DE::List<u32>* list = DE::Memory::allocate<List<u32>>();
	list->init();

	test_expected_uint(list->getLength(), 0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	test_expected_uint(list->getLength(), 3);

	DE::List<u32>* list2 = DE::Memory::allocate<List<u32>>();
	list2->init(*list);

	test_expected_uint(list2->getLength(), 3);

	test_expected_uint(list2->popBack(), 2);
	test_expected_uint(list2->popBack(), 1);
	test_expected_uint(list2->popBack(), 0);

	test_expected_uint(list2->getLength(), 0);

	test_expected_uint(list->popBack(), 2);
	test_expected_uint(list->popBack(), 1);
	test_expected_uint(list->popBack(), 0);

	test_expected_uint(list->getLength(), 0);

	list->pushFront(0);
	list->pushFront(1);
	list->pushFront(2);

	test_expected_uint(list->getLength(), 3);

	test_expected_uint(list->get(2), 0);
	test_expected_uint(list->get(1), 1);
	test_expected_uint(list->get(0), 2);

	test_expected_uint(list->popFront(), 2);
	test_expected_uint(list->popFront(), 1);
	test_expected_uint(list->popFront(), 0);

	test_expected_uint(list->getLength(), 0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	test_expected_uint(list->get(0), 0);
	test_expected_uint(list->get(1), 1);
	test_expected_uint(list->get(2), 2);

	test_expected_uint(list->getLength(), 3);

	list->removeAt(1);

	test_expected_uint(list->get(0), 0);
	test_expected_uint(list->get(1), 2);

	test_expected_uint(list->getLength(), 2);

	list->removeAt(0);

	test_expected_uint(list->get(0), 2);

	list->removeAt(0);

	test_expected_uint(list->getLength(), 0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	test_expected_uint(list->getLength(), 3);

	list->insert(0, 3);
	list->insert(3, 4);

	test_expected_uint(list->getLength(), 5);

	test_expected_uint(list->get(0), 3);
	test_expected_uint(list->get(1), 0);
	test_expected_uint(list->get(2), 1);
	test_expected_uint(list->get(3), 4);
	test_expected_uint(list->get(4), 2);

	list->clear();

	test_expected_uint(list->getLength(), 0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	test_expected_uint(list->getLength(), 3);

	List<u32>::ListIterator it = list->getIterator();

	test_expected_uint(it.get(), 0);
	test_expected_bool(it.hasNext(), true);
	it.next();
	test_expected_uint(it.get(), 1);
	test_expected_bool(it.hasNext(), true);
	it.next();
	test_expected_uint(it.get(), 2);
	test_expected_bool(it.hasNext(), false);

	it.setReverse(true);

	test_expected_uint(it.get(), 2);
	test_expected_bool(it.hasNext(), true);
	it.next();
	test_expected_uint(it.get(), 1);
	test_expected_bool(it.hasNext(), true);
	it.next();
	test_expected_uint(it.get(), 0);
	test_expected_bool(it.hasNext(), false);

	list->clear();

	test_expected_uint(list->getLength(), 0);

	// sort

	list->pushBack(6);
	list->pushBack(5);
	list->pushBack(4);
	list->pushBack(3);
	list->pushBack(2);
	list->pushBack(1);
	list->pushBack(0);

	list->sort();

	test_expected_uint(list->get(0), 0);
	test_expected_uint(list->get(1), 1);
	test_expected_uint(list->get(2), 2);
	test_expected_uint(list->get(3), 3);
	test_expected_uint(list->get(4), 4);
	test_expected_uint(list->get(5), 5);
	test_expected_uint(list->get(6), 6);

	list->clear();

	list->pushBack(50);
	list->pushBack(70);
	list->pushBack(3);
	list->pushBack(100);
	list->pushBack(2);
	list->pushBack(1);
	list->pushBack(0);

	list->sort();

	test_expected_uint(list->get(0), 0);
	test_expected_uint(list->get(1), 1);
	test_expected_uint(list->get(2), 2);
	test_expected_uint(list->get(3), 3);
	test_expected_uint(list->get(4), 50);
	test_expected_uint(list->get(5), 70);
	test_expected_uint(list->get(6), 100);

	list->clear();

	/*i32 size = 10000;

	for (i32 i = size - 1; i >= 0; i--) {
		list->pushBack(i);
	}

	list->sort();

	for (i32 i = 0; i < size; ++i) {
		test_expected_uint(list->get(i), i);
	}*/

	DE::Memory::free(list);
	DE::Memory::free(list2);

	DE::Memory::free();

	summary();

	return 0;
}
