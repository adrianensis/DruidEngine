#include "Debug.h"
#include "BasicTypes.h"

#include <iostream>
#include <string>


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
