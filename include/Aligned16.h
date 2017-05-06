#ifndef INCLUDE_ALIGNED16_H_
#define INCLUDE_ALIGNED16_H_

// for sse auto-vectorization

namespace DE {

class __attribute__((aligned (16))) Aligned16 {

};

} /* namespace DE */

#endif /* INCLUDE_ALIGNED16_H_ */
