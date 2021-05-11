#pragma once

#include "Core/BasicTypes.hpp"

namespace Hash {
	static u32 hashString(std::string key) {
		std::hash<std::string> hash_fn;
		u32 hashString = hash_fn(key);
		return hashString;
	}
}