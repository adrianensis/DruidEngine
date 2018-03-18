#ifndef HASH_H_
#define HASH_H_

#include "BasicTypes.h"

namespace DE {

class Hash {

private:

    const u64 mHash = reinterpret_cast<u64>(this);

public:

    Hash() = default;
    virtual ~Hash() = default;

    virtual u64 hash() const{
        return mHash;
    };

    bool operator==(const Hash& rhs) const {
        return this->mHash == rhs.mHash;
    }

    bool operator!=(const Hash& rhs) {
        return !((*this) == rhs);
    }
};

} /* namespace DE */

#endif /* HASH_H_ */
