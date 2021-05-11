#pragma once

#include "Core/BasicTypes.hpp"
#include <type_traits>

#define NONE(...)

// --------------------------------------------------------
// CLASS - METADATA MACROS
// --------------------------------------------------------

#define CLASS_MACRO_BASE(ClassName, ...)\
class ClassName;\
class ClassName ## _PARENT: public __VA_ARGS__ {\
	GENERATE_METADATA(ClassName)\
};\
class ClassName: public ClassName ## _PARENT\

#define SINGLETON(...) public Singleton<__VA_ARGS__>

#define CLASS(ClassName, ...) CLASS_MACRO_BASE(ClassName, __VA_ARGS__)

#define GENERATE_ID_STATIC(...)\
	static ClassId getClassIdStatic(){\
		static ClassId classId = Hash::hashString(# __VA_ARGS__); return classId;\
	};

#define GENERATE_ID_VIRTUAL(...)\
	ClassId getClassId() const override {\
		return __VA_ARGS__ ## _PARENT::getClassIdStatic();\
	};

#define GENERATE_NAME_STATIC(...)\
	static std::string getClassNameStatic(){\
		static std::string className = std::string(# __VA_ARGS__);\
		return className;\
	};

#define GENERATE_NAME_VIRTUAL(...)\
	std::string getClassName() const override {\
		return __VA_ARGS__ ## _PARENT::getClassNameStatic();\
	};

#define GENERATE_DYNAMIC_DESTRUCTOR_VIRTUAL(...)\
	virtual void dynamicDestructor() override { this->~__VA_ARGS__(); };

#define GENERATE_METADATA(...)\
	public:\
	GENERATE_NAME_STATIC(__VA_ARGS__);\
	GENERATE_NAME_VIRTUAL(__VA_ARGS__);\
	GENERATE_ID_STATIC(__VA_ARGS__);\
	GENERATE_ID_VIRTUAL(__VA_ARGS__);\
	private:

// --------------------------------------------------------
// MEMBERS, GETTERS AND SETTERS
// --------------------------------------------------------

#define REMOVE_REF(Class) std::remove_reference<Class>::type
#define IS_POINTER(Class) std::is_pointer<REMOVE_REF(Class)>::value
#define IS_ARITHMETIC(Class) std::is_arithmetic<REMOVE_REF(Class)>::value

#define COND_TYPE(Bool, T1, T2) std::conditional<Bool, T1, T2>::type

#define GETTER_TYPE(Var)\
	COND_TYPE(\
		IS_POINTER(decltype(Var)),\
		std::add_const<REMOVE_REF(decltype(Var))>::type,\
		COND_TYPE(\
			IS_ARITHMETIC(decltype(Var)),\
			REMOVE_REF(decltype(Var)),\
			decltype(Var)\
		)\
	)

#define SETTER_TYPE(Var) GETTER_TYPE(Var) // NOTE: It's the same as the getter.

#define GET(BaseName)\
	GETTER_TYPE(m ## BaseName) get ## BaseName() const { return m ## BaseName; };

#define GETREF(BaseName)\
	std::add_lvalue_reference<GETTER_TYPE(m ## BaseName)>::type get ## BaseName() { return m ## BaseName; };

#define GETREF_CONST(BaseName)\
	std::add_lvalue_reference<std::add_const<GETTER_TYPE(m ## BaseName)>::type>::type get ## BaseName() const { return m ## BaseName; };

#define SET(BaseName)\
	void set ## BaseName (SETTER_TYPE(m ## BaseName) new ## BaseName){ m ## BaseName = new ## BaseName; };

#define GET_SET(BaseName) GET(BaseName) SET(BaseName)
#define GETREF_SET(BaseName) GETREF(BaseName) SET(BaseName)
#define GETREF_CONST_SET(BaseName) GETREF_CONST(BaseName) SET(BaseName)

#define MEMBER_BASE(BaseName, ...)\
	__VA_ARGS__ m ## BaseName = {};

#define MEMBER(BaseName, AccessorMacroName, Visibility, ...)\
	Visibility:\
	MEMBER_BASE(BaseName, __VA_ARGS__) public: AccessorMacroName(BaseName) Visibility:

#define PUB(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, public, __VA_ARGS__)
#define PRO(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, protected, __VA_ARGS__)
#define PRI(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, private, __VA_ARGS__)

// --------------------------------------------------------
// FOR LOOPS
// --------------------------------------------------------

#define FOR_RANGE(it, start, size) for (i32 it = start; it < size; ++it)
#define FOR_RANGE_COND(it, start, size, condition) for (i32 it = start; it < size && condition; ++it)

#define FOR_ARRAY(it, array) FOR_RANGE(it, 0, array->getLength())
#define FOR_ARRAY_COND(it, array, condition) for (u32 it = 0; it < array->getLength() && condition; ++it)

#define FOR_LIST(it, list) for (auto it = list->getIterator(); !it.isNull(); it.next())
#define FOR_LIST_COND(it, list, cond) for (auto it = list->getIterator(); (!it.isNull()) && cond; it.next())

