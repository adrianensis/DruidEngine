#include "Druid.h"

using namespace DE;

int main() {


	test(DE::List<u32>);

	DE::Memory::init();

	DE::List<u32>* list = DE::Memory::allocate<List<u32>>();
	list->init();

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	DE::List<u32>* list2 = DE::Memory::allocate<List<u32>>();
	list2->init(*list);

	expected_uint(list2->getLength(),3);

	expected_uint(list2->popBack(),2);
	expected_uint(list2->popBack(),1);
	expected_uint(list2->popBack(),0);

	expected_uint(list2->getLength(),0);

	expected_uint(list->popBack(),2);
	expected_uint(list->popBack(),1);
	expected_uint(list->popBack(),0);

	expected_uint(list->getLength(),0);

	list->pushFront(0);
	list->pushFront(1);
	list->pushFront(2);

	expected_uint(list->getLength(),3);

	expected_uint(list->get(2),0);
	expected_uint(list->get(1),1);
	expected_uint(list->get(0),2);

	expected_uint(list->popFront(),2);
	expected_uint(list->popFront(),1);
	expected_uint(list->popFront(),0);

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->get(0),0);
	expected_uint(list->get(1),1);
	expected_uint(list->get(2),2);

	expected_uint(list->getLength(),3);

	list->remove(1);

	expected_uint(list->get(0),0);
	expected_uint(list->get(1),2);

	expected_uint(list->getLength(),2);

	list->remove(0);

	expected_uint(list->get(0),2);

	list->remove(0);

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	list->insert(0,3);
	list->insert(3,4);

	expected_uint(list->getLength(),5);

	expected_uint(list->get(0),3);
	expected_uint(list->get(1),0);
	expected_uint(list->get(2),1);
	expected_uint(list->get(3),4);
	expected_uint(list->get(4),2);

	list->clear();

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	List<u32>::Iterator it = list->getIterator();

	expected_uint(it.get(),0);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),1);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),2);
	expected_bool(it.hasNext(),false);

	it.setReverse(true);

	expected_uint(it.get(),2);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),1);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),0);
	expected_bool(it.hasNext(),false);


	list->clear();

	expected_uint(list->getLength(),0);

	// sort

	list->pushBack(6);
	list->pushBack(5);
	list->pushBack(4);
	list->pushBack(3);
	list->pushBack(2);
	list->pushBack(1);
	list->pushBack(0);

	list->sort();

	expected_uint(list->get(0),0);
	expected_uint(list->get(1),1);
	expected_uint(list->get(2),2);
	expected_uint(list->get(3),3);
	expected_uint(list->get(4),4);
	expected_uint(list->get(5),5);
	expected_uint(list->get(6),6);

	list->clear();

	list->pushBack(50);
	list->pushBack(70);
	list->pushBack(3);
	list->pushBack(100);
	list->pushBack(2);
	list->pushBack(1);
	list->pushBack(0);

	list->sort();

	expected_uint(list->get(0),0);
	expected_uint(list->get(1),1);
	expected_uint(list->get(2),2);
	expected_uint(list->get(3),3);
	expected_uint(list->get(4),50);
	expected_uint(list->get(5),70);
	expected_uint(list->get(6),100);

	DE::Memory::free<DE::List<u32>>(list);
	DE::Memory::free<DE::List<u32>>(list2);

	DE::Memory::free();

	summary();

	return 0;
}
