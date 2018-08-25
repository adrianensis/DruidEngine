#include "Druid.h"
#include <string>

using namespace DE;

int main() {


	test(Tree<i32>);

	DE::Memory::init();

	Tree<i32>* tree = DE::Memory::allocate<Tree<i32>>();

	tree->init();

	expected_uint(tree->getLength(),0);

	tree->add(0);
	expected_uint(tree->getLength(),1);
	tree->add(1);
	expected_uint(tree->getLength(),2);
	tree->add(2);
	expected_uint(tree->getLength(),3);
	tree->add(3);
	expected_uint(tree->getLength(),4);
	tree->add(-1);
	expected_uint(tree->getLength(),5);
	tree->add(-2);
	expected_uint(tree->getLength(),6);
	tree->add(-3);
	expected_uint(tree->getLength(),7);

	tree->remove(-1);
	expected_uint(tree->getLength(),6);
	tree->remove(1);
	expected_uint(tree->getLength(),5);
	tree->remove(3);
	expected_uint(tree->getLength(),4);
	tree->remove(-2);
	expected_uint(tree->getLength(),3);
	tree->remove(2);
	expected_uint(tree->getLength(),2);
	tree->remove(0);
	expected_uint(tree->getLength(),1);
	tree->remove(-3);
	expected_uint(tree->getLength(),0);

	tree->clear();

	expected_uint(tree->getLength(),0);

	tree->add(-3);
	expected_uint(tree->getLength(),1);
	tree->add(1);
	expected_uint(tree->getLength(),2);
	tree->add(0);
	expected_uint(tree->getLength(),3);
	tree->add(-1);
	expected_uint(tree->getLength(),4);
	tree->add(3);
	expected_uint(tree->getLength(),5);
	tree->add(-2);
	expected_uint(tree->getLength(),6);
	tree->add(2);
	expected_uint(tree->getLength(),7);

	tree->remove(-1);
	expected_uint(tree->getLength(),6);
	tree->remove(1);
	expected_uint(tree->getLength(),5);
	tree->remove(3);
	expected_uint(tree->getLength(),4);
	tree->remove(-2);
	expected_uint(tree->getLength(),3);
	tree->remove(2);
	expected_uint(tree->getLength(),2);
	tree->remove(0);
	expected_uint(tree->getLength(),1);
	tree->remove(-3);
	expected_uint(tree->getLength(),0);

	DE::Memory::free();

	summary();

	return 0;
}
