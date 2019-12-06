#ifndef DE_FUNCTOR_H_
#define DE_FUNCTOR_H_

#include "DE_Class.h"
#include <functional>

namespace DE {

class Functor {

private:

  using voidFunction = std::function<void()>;

  std::function<void()> mCallback;

public:
  Functor() {};
  ~Functor() {};

  void execute();

  DE_SET(voidFunction, Callback);
};

} /* namespace DE */

#endif /* DE_FUNCTOR_H_ */
