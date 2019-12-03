#ifndef DE_SINGLETON_H
#define DE_SINGLETON_H

#include "Basic.h"
#include "Memory.h"

namespace DE {

template<class T>
class Singleton{

protected:
  static T* mInstance;

public:
  Singleton() = default;

  ~Singleton() = default;

  static T* getInstance(){
  	if( ! mInstance){
  		mInstance = Memory::allocate<T>();
  	}

  	return mInstance;
  };

};

template<class T>
T* Singleton<T>::mInstance = nullptr;

}; /* namespace DE */

#endif /* DE_SINGLETON_H */
