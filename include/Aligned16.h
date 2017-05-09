#ifndef ALIGNED16_H_
#define ALIGNED16_H_

// for sse auto-vectorization

namespace DE {

class __attribute__((aligned (16))) Aligned16 {

};

} /* namespace DE */

#endif /* ALIGNED16_H_ */
