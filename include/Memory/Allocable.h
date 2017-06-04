#ifndef ALLOCABLE_H_
#define ALLOCABLE_H_

#include "Allocator.h"

namespace DE {


class Allocable {

public:

  Allocator* mAllocator;

  Allocable(){};
  virtual ~Allocable(){};
  void init(Allocator* allocator){ mAllocator = allocator; };
  void init(const Allocable& other){ mAllocator = other.mAllocator; };

};

template<class T>
T* allocate(Allocator& allocator, u32 alignment){
  Allocable* obj = static_cast<Allocable*>(allocator.allocate(sizeof(T), alignment));
  obj->init(&allocator);
  return static_cast<T*>(obj);
};

template<class T>
T* allocate(Allocator& allocator){
  Allocable* obj = static_cast<Allocable*>(allocator.allocate(sizeof(T)));
  obj->init(&allocator);
  return static_cast<T*>(obj);
};

} /* namespace DE */

#endif /* ALLOCABLE_H_ */
