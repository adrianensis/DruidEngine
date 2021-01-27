#include "Core/Hash.hpp"

namespace DE {

template<>
u64 Hash::hash<std::string>(std::string key) {
	std::hash<std::string> hash_fn;
	u64 hashString = hash_fn(key);
	return hashString;
}

template<>
u64 Hash::hash<const c8*>(const c8 *key) {
	std::string str(key);
	std::hash<std::string> hash_fn;
	u64 hashString = hash_fn(str);
	return hashString;
}

}
