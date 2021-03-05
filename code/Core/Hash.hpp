#pragma once

#include "Core/BasicTypes.hpp"
#include "Assert/Assert.hpp"
#include <string>

namespace DE {

/*!
 \brief Gives objects a hash number (u64).
 */
class Hash {

private:

	const u64 mHash = reinterpret_cast<u64>(this);

public:

	Hash() = default;
	virtual ~Hash() = default;

	virtual u64 hash() const {
		return mHash;
	};

	bool operator==(const Hash &rhs) const {
		return this->mHash == rhs.mHash;
	}

	bool operator!=(const Hash &rhs) {
		return !((*this) == rhs);
	}

	template<class H>
	static u64 hash(H key) {
		if constexpr (std::is_base_of<Hash, H>::value) {
			return ((Hash*) &key)->hash();
		} else if constexpr (std::is_integral<H>::value || std::is_pointer<H>::value) {
			return (u64) key;
		} else {
			DE_ASSERT(false, "Object or Class is not Hash compatible.");
		}
	}
};

template<> u64 Hash::hash<String>(String key);
template<> u64 Hash::hash<const c8 *>(const c8 * key);

}

