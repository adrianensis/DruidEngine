#ifndef HASH_H_
#define HASH_H_

#include "BasicTypes.h"

namespace DE {

class Hash {

private:

    const u64 mHash = reinterpret_cast<u64>(this);

public:

    Hash() {};
    ~Hash() {};

    virtual u64 hash() const{
        return mHash;
    };
};

} /* namespace DE */

#endif /* HASH_H_ */
