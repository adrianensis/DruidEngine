#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

#define test(theClass) std::chrono::duration<double> total; auto t = std::chrono::high_resolution_clock::now(); std::ofstream outputFile; std::ostringstream output; outputFile.open((std::string(#theClass)+".txt").c_str()); output << std::endl << std::endl << std::endl <<   "===========    " << #theClass << "    ===========" << std::endl << std::endl; theClass __test__aux; bool __test__bool; float __test__float; int __test__ok = 0; int __test__total = 0; bool __test__b
#define title(text) output << std::endl << std::endl << std::endl <<   "===========    " << text << "    ===========" << std::endl << std::endl
#define tick() t=std::chrono::high_resolution_clock::now()
#define tock() total+=std::chrono::high_resolution_clock::now()-t

#define blank() output << std::endl
#define show(x) output << #x << " = " << (x) << std::endl << std::endl

#define __PRECISION std::setprecision(10)

#define expected_any(aux,x,exp) tick(); aux = (x); tock(); __test__total++; __test__b = (aux==(exp)); if (__test__b) { __test__ok++; } output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << aux << std::endl << "\t" << "expected: " << __PRECISION << (exp) << std::endl << std::endl

#define expected_bool(x,exp) expected_any(__test__bool,x,exp)
#define expected_num(x,exp) expected_any(__test__float,x,exp)
#define expected_num_eps(x,exp,eps) tick(); __test__float = (x); tock(); __test__total++; __test__b = (fabsf(__test__float-(exp)) < eps); if (__test__b) { __test__ok++; } output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << __test__float << std::endl << "\t" << "expected: " << __PRECISION << (exp) << std::endl << "\t" << "epsilon: " << __PRECISION << (eps) << std::endl << std::endl
#define expected(x,exp) expected_any(__test__aux,x,exp)

#define summary() output << std::endl << total.count()*1000.0 << " " << __test__ok << " " <<(__test__total-__test__ok) << std::endl; outputFile << output.str(); std::cout << output.str(); outputFile.close()


#endif /* INCLUDE_TEST_H_ */
