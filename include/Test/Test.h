#ifndef DE_TEST_H
#define DE_TEST_H

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include "Basic.h"

using namespace DE;

#define test(theClass) std::chrono::duration<double> __test__totaltime; auto t = std::chrono::high_resolution_clock::now(); std::ofstream __test__outputFile; std::ostringstream __test__output; __test__outputFile.open((std::string(#theClass)+".txt").c_str()); __test__output << std::endl << std::endl << std::endl <<   "===========  " << #theClass << "  ===========" << std::endl << std::endl; theClass __test__any; bool __test__bool; f32 __test__float; f64 __test__float_precision; i64 __test__sint; u64 __test__uint; int __test__ok = 0; int __test__total = 0; bool __test__b
#define title(text) __test__output << std::endl << std::endl << std::endl <<   "===========  " << text << "  ===========" << std::endl << std::endl
#define tic() t=std::chrono::high_resolution_clock::now()
#define toc()__test__totaltime+=std::chrono::high_resolution_clock::now()-t

#define blank() __test__output << std::endl
#define show(x) __test__output << #x << " = " << (x) << std::endl << std::endl

#define __PRECISION std::setprecision(10)

#define expected_any(aux,x,exp) tic(); aux = (x); toc(); __test__total++; __test__b = (aux==(exp)); if (__test__b) { __test__ok++; } __test__output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << aux << std::endl << "\t" << "expected: " << __PRECISION << (exp) << std::endl << std::endl

#define expected_bool(x,exp) expected_any(__test__bool,x,exp)
#define expected_float(x,exp) expected_any(__test__float,x,exp)
#define expected_sint(x,exp) expected_any(__test__sint,x,exp)
#define expected_uint(x,exp) expected_any(__test__uint,x,exp)
#define expected_float_eps(x,exp,eps) tic(); __test__float_precision = (x); toc(); __test__total++; __test__b = ( std::max(0.0f,fabsf(__test__float_precision)-fabsf(exp)) < fabsf(eps)); if (__test__b) { __test__ok++; } __test__output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << __test__float << std::endl << "\t" << "expected: " << __PRECISION << (exp) << std::endl << "\t" << "epsilon: " << __PRECISION << (eps) << std::endl << std::endl
#define expected(x,exp) expected_any(__test__any,x,exp)

#define summary() __test__output << std::endl <<__test__totaltime.count()*1000.0 << " " << __test__ok << " " <<(__test__total-__test__ok) << std::endl; __test__outputFile << __test__output.str(); std::cout << __test__output.str(); __test__outputFile.close()


#endif /* DE_TEST_H */
