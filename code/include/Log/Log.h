#ifndef DE_LOG_H
#define DE_LOG_H

#include <string>
#include <iostream>

#include "BasicTypes.h"

namespace DE {

static const std::string emptyMessage = "";

void log(const std::string file, u32 line, const std::string function, const std::string message = emptyMessage);

void echo(const std::string& message);

template <class T>
void var(const std::string& varname, T var) {
  std::cout << "DE_VAR > " << varname << " : " << var << std::endl;
};

template <class T>
void val(const T& var) {
  std::cout << "DE_VAL > " << var << std::endl;
};

void error(const std::string& message);

void brline();

#ifdef DE_ENABLE_LOGS
  #define LOG(x) log(__FILE__, __LINE__, __PRETTY_FUNCTION__, x);
  #define TRACE() log(__FILE__, __LINE__, __PRETTY_FUNCTION__);
  #define TO_STR(s) #s
  #define ECHO(x) echo(x);
  #define VAR(T,x) var<T>(#x, x);
  #define VAL(T,x) val<T>(x);
  #define ERROR(x) error(x);
  #define BRLINE() brline();
#else
  #define LOG()
  #define TRACE()
  #define TO_STR(s)
  #define ECHO(x)
  #define VAR(T,x)
  #define VAL(T,x)
  #define ERROR(x)
  #define BRLINE()
#endif

} /* namespace DE */

#endif /* DE_LOG_H */
