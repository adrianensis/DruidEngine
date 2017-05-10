#include "Debug.h"

namespace DE {

namespace Debug {

void echo(string message) {
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
