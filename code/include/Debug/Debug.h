#ifndef DE_DEBUG_H
#define DE_DEBUG_H

#include <string>
#include <iostream>

namespace DE {

void echo(const std::string& message);

template <class T>
void var(const std::string& varname, T var) {
  std::cout << "D > " << varname << " : " << var << std::endl;
};

template <class T>
void val(const T& var) {
  std::cout << "D > " << var << std::endl;
};

void error(const std::string& message);

void brline();

#ifdef DE_DEBUG
  #define TO_STR(s) #s
  #define ECHO(x) echo(x);
  #define VAR(T,x) var<T>(#x, x);
  #define VAL(T,x) val<T>(x);
  #define ERROR(x) error(x);
  #define BRLINE() brline();
#else
  #define TO_STR(s)
  #define ECHO(x)
  #define VAR(T,x)
  #define VAL(T,x)
  #define ERROR(x)
  #define BRLINE()
#endif

} /* namespace DE */

#endif /* DE_DEBUG_H */
