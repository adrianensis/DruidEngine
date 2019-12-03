#ifndef DE_FUNCTOR_H_
#define DE_FUNCTOR_H_

namespace DE {

class Functor {

public:
  Functor() {};
  ~Functor() {};

  virtual void execute() = 0;
};

} /* namespace DE */

#endif /* DE_FUNCTOR_H_ */
