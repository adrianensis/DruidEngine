#include "Druid.h"
#include <string>

using namespace DE;

int main() {

	using HashMapu32u32 = HashMap<u32,u32>;

	test(HashMapu32u32);

	DE::Memory::init();

	DE::HashMap<u32,u32>* map = DE::Memory::allocate<HashMap<u32,u32>>();

	map->init();

	expected_uint(map->getLength(),0);

	map->set(0,0);
	expected_uint(map->getLength(),1);
	expected_uint(map->get(0),0);
	map->set(10,10);
	expected_uint(map->getLength(),2);
	expected_uint(map->get(10),10);
	map->set(10,100);
	expected_uint(map->getLength(),2);
	expected_uint(map->get(10),100);

	map->remove(0);
	expected_uint(map->getLength(),1);
	map->remove(10);
	expected_uint(map->getLength(),0);

	DE::HashMap<std::string,u32>* mapStr = DE::Memory::allocate<HashMap<std::string,u32>>();

	mapStr->init();

	expected_uint(mapStr->getLength(),0);

	std::string str0("test0");
	std::string str10("test10");

	mapStr->set(str0,0);
	expected_uint(mapStr->getLength(),1);
	expected_uint(mapStr->get(str0),0);
	mapStr->set(str10,10);
	expected_uint(mapStr->getLength(),2);
	expected_uint(mapStr->get(str10),10);

	mapStr->remove(str0);
	expected_uint(mapStr->getLength(),1);
	mapStr->remove(str10);
	expected_uint(mapStr->getLength(),0);

	DE::HashMap<const char*,u32>* mapStrLiteral = DE::Memory::allocate<HashMap<const char*,u32>>();

	mapStrLiteral->init();

	expected_uint(mapStrLiteral->getLength(),0);

	char* strLiteral0 = "test0";
	char* strLiteral10 = "test10";

	mapStrLiteral->set(strLiteral0,0);
	expected_uint(mapStrLiteral->getLength(),1);
	expected_uint(mapStrLiteral->get(strLiteral0),0);
	mapStrLiteral->set(strLiteral10,10);
	expected_uint(mapStrLiteral->getLength(),2);
	expected_uint(mapStrLiteral->get(strLiteral10),10);

	mapStrLiteral->remove(strLiteral0);
	expected_uint(mapStrLiteral->getLength(),1);
	mapStrLiteral->remove(strLiteral10);
	expected_uint(mapStrLiteral->getLength(),0);

	DE::HashMap<u32*,u32>* mapPtr = DE::Memory::allocate<HashMap<u32*,u32>>();

	mapPtr->init();

	expected_uint(mapPtr->getLength(),0);

	u32 a = 0;
	u32 b = 10;

	u32* ptr0 = &a;
	u32* ptr10 = &b;

	mapPtr->set(ptr0,a);
	expected_uint(mapPtr->getLength(),1);
	expected_uint(mapPtr->get(ptr0),a);
	mapPtr->set(ptr10,b);
	expected_uint(mapPtr->getLength(),2);
	expected_uint(mapPtr->get(ptr10),b);

	mapPtr->remove(ptr0);
	expected_uint(mapPtr->getLength(),1);
	mapPtr->remove(ptr10);
	expected_uint(mapPtr->getLength(),0);

	DE::HashMap<i32,i32>* mapSigned = DE::Memory::allocate<HashMap<i32,i32>>();

	mapSigned->init();

	expected_uint(mapSigned->getLength(),0);

	mapSigned->set(0,0);
	expected_uint(mapSigned->getLength(),1);
	expected_sint(mapSigned->get(0),0);
	mapSigned->set(10,10);
	expected_uint(mapSigned->getLength(),2);
	expected_sint(mapSigned->get(10),10);
	mapSigned->set(-1,-1);
	expected_uint(mapSigned->getLength(),3);
	expected_sint(mapSigned->get(-1),-1);
	mapSigned->set(-10,-10);
	expected_uint(mapSigned->getLength(),4);
	expected_sint(mapSigned->get(-10),-10);

	mapSigned->remove(0);
	expected_uint(mapSigned->getLength(),3);
	mapSigned->remove(10);
	expected_uint(mapSigned->getLength(),2);
	mapSigned->remove(-1);
	expected_uint(mapSigned->getLength(),1);
	mapSigned->remove(-10);
	expected_uint(mapSigned->getLength(),0);

	DE::Memory::free<HashMap<u32,u32>>(map);
	DE::Memory::free<DE::HashMap<std::string,u32>>(mapStr);
	DE::Memory::free<DE::HashMap<const char*,u32>>(mapStrLiteral);
	DE::Memory::free<DE::HashMap<u32*,u32>>(mapPtr);
	DE::Memory::free<DE::HashMap<i32,i32>>(mapSigned);

	DE::Memory::free();

	summary();

	return 0;
}
