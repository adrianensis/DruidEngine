#include "Druid.h"
#include <string>

using namespace DE;

int main() {


	test(Tree<u32>);

	DE::Memory::init();

    Tree<u32>* tree = DE::Memory::allocate<Tree<u32>>();

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

    tree->remove(3);
    expected_uint(tree->getLength(),3);

    tree->clear();

    expected_uint(tree->getLength(),0);

	DE::Memory::free();

	summary();

	return 0;
}
