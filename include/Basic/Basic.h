#ifndef BASIC_H_
#define BASIC_H_

#include "BasicTypes.h"
#include "DE_Class.h"
#include "Hash.h"
#include "Assert.h"
#include "Debug.h"
#include "MathUtils.h"
#include <type_traits>

#define id_to_str(str) #str
#define CLASS_ID(classname) id_to_str(id_##classname)

#endif /* BASIC_H_ */
