#include "Debug.h"

#include <iostream>
#include <string>
#include "BasicTypes.h"

namespace DE {

namespace Debug {

using namespace std;


template<typename T>
void echo(T message) {
  #ifdef DE_DEBUG
  cout << "D > " << message << endl;
  #endif
};

void var(string varname, auto var) {
  #ifdef DE_DEBUG
  cout << "D > " << varname << " " << var << endl;
  #endif
};

void var(auto var) {
  #ifdef DE_DEBUG
  cout << "D > " << var << endl;
  #endif
};

void error(string message) {
  #ifdef DE_DEBUG
  cout << "ERR > " << message << endl;
  #endif
};

void br() {
  #ifdef DE_DEBUG
  cout << endl;
  #endif
};


}

}
