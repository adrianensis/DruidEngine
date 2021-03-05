#include "Core/Hash.hpp"

namespace DE {

template<>
u64 Hash::hash<String>(String key) {
	std::hash<String> hash_fn;
	u64 hashString = hash_fn(key);
	return hashString;
}

template<>
u64 Hash::hash<const c8*>(const c8 *key) {
	String str(key);
	std::hash<String> hash_fn;
	u64 hashString = hash_fn(str);
	return hashString;
}

}
