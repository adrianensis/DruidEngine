#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>
#include <iostream>
#include "BasicTypes.h"

using namespace std;

namespace DE {


void echo(string message);

template <class T>
void var(string varname, T var) {
  #ifdef DE_DEBUG
  cout << "D > " << varname << " : " << var << endl;
  #endif
};

template <class T>
void val(T var) {
  #ifdef DE_DEBUG
  cout << "D > " << var << endl;
  #endif
};

void error(string message);

void brline();


#define ECHO(x) echo(x);
#define VAR(T,x) var<T>(#x, x);
#define VAL(T,x) val<T>(x);
#define ERROR(x) error(x);
#define BRLINE(x) brline();

} /* namespace DE */

#endif /* DEBUG_H_ */
