#ifndef DE_DEBUG_H
#define DE_DEBUG_H

#include <string>
#include <iostream>

namespace DE {

void echo(const std::string& message);

template <class T>
void var(const std::string& varname, T var) {
  #ifdef DE_DEBUG
  std::cout << "D > " << varname << " : " << var << std::endl;
  #endif
};

template <class T>
void val(const T& var) {
  #ifdef DE_DEBUG
  std::cout << "D > " << var << std::endl;
  #endif
};

void error(const std::string& message);

void brline();

#define TO_STR(s) #s
#define ECHO(x) echo(x);
#define VAR(T,x) var<T>(#x, x);
#define VAL(T,x) val<T>(x);
#define ERROR(x) error(x);
#define BRLINE() brline();

} /* namespace DE */

#endif /* DE_DEBUG_H */
