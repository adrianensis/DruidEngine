#ifndef DE_TEST_H
#define DE_TEST_H

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <math.h> /* isnan */
#include "Basic.h"

using namespace DE;

#define DE_test(theClass) float __test__totaltime; std::chrono::duration<double, std::milli> elapsed; auto t = std::chrono::high_resolution_clock::now(); std::ostringstream __test__output; __test__output << std::endl << std::endl << std::endl <<   "===========  " << #theClass << "  ===========" << std::endl << std::endl; theClass __test__any; bool __test__bool; f32 __test__float; f64 __test__float_precision; i64 __test__sint; u64 __test__uint; int __test__ok = 0; int __test__total = 0; bool __test__b
#define DE_test_title(text) __test__output << std::endl << std::endl << std::endl <<   "===========  " << text << "  ===========" << std::endl << std::endl

#define DE_test_tick() t=std::chrono::high_resolution_clock::now()
#define DE_test_tock() elapsed = std::chrono::high_resolution_clock::now()-t; __test__totaltime += elapsed.count()

#define DE_test_blank() __test__output << std::endl
#define DE_test_show(x) __test__output << #x << " = " << (x) << std::endl << std::endl

#define __PRECISION std::setprecision(10)

#define DE_test_expected_any(aux,x,exp) DE_test_tick(); aux = (x); DE_test_tock(); __test__total++; __test__b = (aux==(exp)); if (__test__b){ __test__ok++; } __test__output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << aux << std::endl << "\t" << "DE_test_expected: " << __PRECISION << (exp) << std::endl << std::endl

#define DE_test_expected_bool(x,exp) DE_test_expected_any(__test__bool,x,exp)
#define DE_test_expected_float(x,exp) DE_test_expected_any(__test__float,x,exp)
#define DE_test_expected_sint(x,exp) DE_test_expected_any(__test__sint,x,exp)
#define DE_test_expected_uint(x,exp) DE_test_expected_any(__test__uint,x,exp)
#define DE_test_expected_float_eps(x,exp,eps) DE_test_tick(); __test__float_precision = (x); DE_test_tock(); __test__total++; __test__b = ( std::max(0.0f,fabsf(__test__float_precision)-fabsf(exp)) < fabsf(eps)); if (__test__b){ __test__ok++; } __test__output  << (__test__b ? "[ OK ]" : "[ FAIL ]") << "\t" << #x << std::endl << "\t" << "result: " << __PRECISION << __test__float << std::endl << "\t" << "DE_test_expected: " << __PRECISION << (exp) << std::endl << "\t" << "epsilon: " << __PRECISION << (eps) << std::endl << std::endl
#define DE_test_expected(x,exp) DE_test_expected_any(__test__any,x,exp)

#define summary() __test__output << std::endl << std::setprecision(4) << (isnan(__test__totaltime) ? 0 : __test__totaltime) << "ms " << __test__ok << " " <<(__test__total-__test__ok) << std::endl; std::cout << __test__output.str();


#endif /* DE_TEST_H */
