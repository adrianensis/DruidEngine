#include "Functor.h"

namespace DE{

void Functor::execute(){
  if(mCallback) mCallback();
}

}
