#include "Debug.h"

#include <iostream>
#include <string>
#include "BasicTypes.h"


using namespace std;

namespace DE {


void echo(string message) {
  #ifdef DE_DEBUG
  cout << "D > " << message << endl;
  #endif
};

void error(string message) {
  #ifdef DE_DEBUG
  cout << "ERR > " << message << endl;
  #endif
};

void brline() { // break line
  #ifdef DE_DEBUG
  cout << endl;
  #endif
};





}
