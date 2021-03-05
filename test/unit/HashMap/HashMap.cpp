#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Containers/HashMap.hpp"
#include <string>

using namespace DE;

int main() {

	DE::Memory::init();

	using HashMapu32u32 = HashMap<u32,u32>;

	DE_test(HashMapu32u32);

	DE::HashMap<u32, u32>* map = DE::DE_NEW<HashMap<u32, u32>>();

	map->init();

	DE_test_expected_uint(map->getLength(), 0);

	map->set(0, 0);
	DE_test_expected_uint(map->getLength(), 1);
	DE_test_expected_uint(map->get(0), 0);
	map->set(10, 10);
	DE_test_expected_uint(map->getLength(), 2);
	DE_test_expected_uint(map->get(10), 10);
	map->set(10, 100);
	DE_test_expected_uint(map->getLength(), 2);
	DE_test_expected_uint(map->get(10), 100);

	map->remove(0);
	DE_test_expected_uint(map->getLength(), 1);
	map->remove(10);
	DE_test_expected_uint(map->getLength(), 0);

	DE::HashMap<String, u32>* mapStr = DE::DE_NEW<HashMap<String, u32>>();

	mapStr->init();

	DE_test_expected_uint(mapStr->getLength(), 0);

	String str0("test0");
	String str10("test10");

	mapStr->set(str0, 0);
	DE_test_expected_uint(mapStr->getLength(), 1);
	DE_test_expected_uint(mapStr->get(str0), 0);
	mapStr->set(str10, 10);
	DE_test_expected_uint(mapStr->getLength(), 2);
	DE_test_expected_uint(mapStr->get(str10), 10);

	mapStr->remove(str0);
	/*DE_test_expected_uint(mapStr->getLength(),1);
	 mapStr->remove(str10);
	 DE_test_expected_uint(mapStr->getLength(),0);*/

	/* DE::HashMap<u32*,u32>* mapPtr = DE::DE_NEW<HashMap<u32*,u32>>();

	 mapPtr->init();

	 DE_test_expected_uint(mapPtr->getLength(),0);

	 u32 a = 0;
	 u32 b = 10;

	 u32* ptr0 = &a;
	 u32* ptr10 = &b;

	 mapPtr->set(ptr0,a);
	 DE_test_expected_uint(mapPtr->getLength(),1);
	 DE_test_expected_uint(mapPtr->get(ptr0),a);
	 mapPtr->set(ptr10,b);
	 DE_test_expected_uint(mapPtr->getLength(),2);
	 DE_test_expected_uint(mapPtr->get(ptr10),b);

	 mapPtr->remove(ptr0);
	 DE_test_expected_uint(mapPtr->getLength(),1);
	 mapPtr->remove(ptr10);
	 DE_test_expected_uint(mapPtr->getLength(),0);

	 DE::HashMap<i32,i32>* mapSigned = DE::DE_NEW<HashMap<i32,i32>>();

	 mapSigned->init();

	 DE_test_expected_uint(mapSigned->getLength(),0);

	 mapSigned->set(0,0);
	 DE_test_expected_uint(mapSigned->getLength(),1);
	 DE_test_expected_sint(mapSigned->get(0),0);
	 mapSigned->set(10,10);
	 DE_test_expected_uint(mapSigned->getLength(),2);
	 DE_test_expected_sint(mapSigned->get(10),10);
	 mapSigned->set(-1,-1);
	 DE_test_expected_uint(mapSigned->getLength(),3);
	 DE_test_expected_sint(mapSigned->get(-1),-1);
	 mapSigned->set(-10,-10);
	 DE_test_expected_uint(mapSigned->getLength(),4);
	 DE_test_expected_sint(mapSigned->get(-10),-10);

	 mapSigned->remove(0);
	 DE_test_expected_uint(mapSigned->getLength(),3);
	 mapSigned->remove(10);
	 DE_test_expected_uint(mapSigned->getLength(),2);
	 mapSigned->remove(-1);
	 DE_test_expected_uint(mapSigned->getLength(),1);
	 mapSigned->remove(-10);
	 DE_test_expected_uint(mapSigned->getLength(),0); */

	DE::DE_FREE(map);
	//DE::DE_FREE(mapStr);
	//DE::DE_FREE(mapStrLiteral);
	/* DE::DE_FREE(mapPtr);
	 DE::DE_FREE(mapSigned); */

	DE::Memory::free();

	summary();

	return 0;
}
