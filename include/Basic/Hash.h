#ifndef HASH_H_
#define HASH_H_

#include "BasicTypes.h"

namespace DE {

class Hash {

public:
    Hash() {};
    ~Hash() {};

    virtual u64 hash(){
        return reinterpret_cast<u64>(this);
    };
};

} /* namespace DE */

#endif /* HASH_H_ */
