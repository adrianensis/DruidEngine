#ifndef DE_FUNCTOR_H_
#define DE_FUNCTOR_H_

#include "DE_Class.hpp"
#include <functional>

namespace DE {

class Functor {

private:

  using voidFunction = std::function<void()>;

  std::function<void()> mCallback;

public:
  Functor(){};
  ~Functor(){};

  void execute();

  void setCallback(voidFunction newCallback ){ mCallback = newCallback; };
};

} /* namespace DE */

#endif /* DE_FUNCTOR_H_ */
