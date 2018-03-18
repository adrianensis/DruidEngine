#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>
#include <iostream>
#include "Basic.h"

using namespace std;

namespace DE {


void echo(const string& message);

template <class T>
void var(const string& varname, T var) {
  #ifdef DE_DEBUG
  cout << "D > " << varname << " : " << var << endl;
  #endif
};

template <class T>
void val(const T& var) {
  #ifdef DE_DEBUG
  cout << "D > " << var << endl;
  #endif
};

void error(const string& message);

void brline();


#define ECHO(x) echo(x);
#define VAR(T,x) var<T>(#x, x);
#define VAL(T,x) val<T>(x);
#define ERROR(x) error(x);
#define BRLINE() brline();

} /* namespace DE */

#endif /* DEBUG_H_ */
