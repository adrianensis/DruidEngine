#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Containers/Tree.hpp"
#include <string>

using namespace DE;

int main() {

	DE::Memory::init();

	test(Tree<i32>);

	Tree<i32>* tree = DE::Memory::allocate<Tree<i32>>();

	tree->init();

	test_expected_uint(tree->getLength(), 0);

	tree->add(0);
	test_expected_uint(tree->getLength(), 1);
	tree->add(1);
	test_expected_uint(tree->getLength(), 2);
	tree->add(2);
	test_expected_uint(tree->getLength(), 3);
	tree->add(3);
	test_expected_uint(tree->getLength(), 4);
	tree->add(-1);
	test_expected_uint(tree->getLength(), 5);
	tree->add(-2);
	test_expected_uint(tree->getLength(), 6);
	tree->add(-3);
	test_expected_uint(tree->getLength(), 7);

	tree->remove(-1);
	test_expected_uint(tree->getLength(), 6);
	tree->remove(1);
	test_expected_uint(tree->getLength(), 5);
	tree->remove(3);
	test_expected_uint(tree->getLength(), 4);
	tree->remove(-2);
	test_expected_uint(tree->getLength(), 3);
	tree->remove(2);
	test_expected_uint(tree->getLength(), 2);
	tree->remove(0);
	test_expected_uint(tree->getLength(), 1);
	tree->remove(-3);
	test_expected_uint(tree->getLength(), 0);

	tree->add(-1);
	tree->add(1);
	tree->add(3);
	tree->add(-2);
	tree->add(2);
	tree->add(0);
	tree->add(-3);

	tree->clear();

	test_expected_uint(tree->getLength(), 0);

	tree->add(-3);
	test_expected_uint(tree->getLength(), 1);
	tree->add(1);
	test_expected_uint(tree->getLength(), 2);
	tree->add(0);
	test_expected_uint(tree->getLength(), 3);
	tree->add(-1);
	test_expected_uint(tree->getLength(), 4);
	tree->add(3);
	test_expected_uint(tree->getLength(), 5);
	tree->add(-2);
	test_expected_uint(tree->getLength(), 6);
	tree->add(2);
	test_expected_uint(tree->getLength(), 7);

	tree->remove(-1);
	test_expected_uint(tree->getLength(), 6);
	tree->remove(1);
	test_expected_uint(tree->getLength(), 5);
	tree->remove(3);
	test_expected_uint(tree->getLength(), 4);
	tree->remove(-2);
	test_expected_uint(tree->getLength(), 3);
	tree->remove(2);
	test_expected_uint(tree->getLength(), 2);
	tree->remove(0);
	test_expected_uint(tree->getLength(), 1);
	tree->remove(-3);
	test_expected_uint(tree->getLength(), 0);

	DE::Memory::free(tree);

	DE::Memory::free();

	summary();

	return 0;
}
