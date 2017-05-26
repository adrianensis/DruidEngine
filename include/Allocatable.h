#ifndef ALLOCATABLE_H_
#define ALLOCATABLE_H_

#include "Allocator.h"

namespace DE {

class Allocatable {

protected:

  Allocator* mAllocator;

public:

  // void* mStart;

  Allocatable(){};
  virtual ~Allocatable(){};
  void init(Allocator* allocator){ mAllocator = allocator; };

};

} /* namespace DE */

#endif /* ALLOCATABLE_H_ */
