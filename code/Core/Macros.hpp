#pragma once

#define NONE(...)

#define REMOVE_REF(Class) typename std::remove_reference<Class>::type
#define REMOVE_POINTER(Class) typename std::remove_pointer<Class>::type
#define IS_POINTER(Class) std::is_pointer<REMOVE_REF(Class)>::value
#define IS_ARITHMETIC(Class) std::is_arithmetic<REMOVE_REF(Class)>::value
#define IS_ENUM(Class) std::is_enum<Class>::value
#define ADD_CONST(Class) typename std::add_const<Class>::type
#define ADD_REFERENCE(Class) typename std::add_lvalue_reference<Class>::type
#define ADD_POINTER(Class) typename std::add_pointer<Class>::type

#define IS_BASE_OF(BaseClass, DerivedClass) std::is_base_of<BaseClass, DerivedClass>::value

#define COND_TYPE(Bool, T1, T2) typename std::conditional<Bool, T1, T2>::type

// --------------------------------------------------------
// NEW - DELETE
// --------------------------------------------------------

#define NEW(...) Memory::newObject<__VA_ARGS__>()
#define INSTANCE_BY_NAME(ClassNameString, ...) ClassManager::getInstance()->instanceByName<__VA_ARGS__>(ClassNameString)
#define DELETE(...) Memory::deleteObject<REMOVE_POINTER(REMOVE_REF(decltype(__VA_ARGS__)))>(__VA_ARGS__);

// --------------------------------------------------------
// MAIN
// --------------------------------------------------------

#define ENGINE_MAIN() \
void __customMain();\
void __initMain()\
{\
	Engine::getInstance()->init();\
}\
void __runMain()\
{\
	Engine::getInstance()->run();\
	Engine::getInstance()->terminate();\
	Engine::deleteInstance();\
}\
int main()\
{\
	REGISTER_CLASSES_BY_NAME();\
	__initMain();\
	__customMain();\
	__runMain();\
	return 0;\
}\
void __customMain()

// --------------------------------------------------------
// CLASS - METADATA MACROS
// --------------------------------------------------------

#define CLASS_MACRO_CONSTRUCTOR(ClassName) \
	ClassName() = default;        \
	~ClassName() OVR = default;

#define CLASS_MACRO_COMMON(ClassName, ...) \
	class ClassName##_PARENT : public __VA_ARGS__           \
	{                                                       \
		GENERATE_METADATA(ClassName)               \
	public:                                                 \
		CLASS_MACRO_CONSTRUCTOR(ClassName##_PARENT)                  \
	protected:                                              \
		using Super = __VA_ARGS__;                          \
	private:\
		SStr ____className = getClassNameStatic();\
	};

#define TEMPLATE_MACRO(Template)\
template<class Template>

#define CLASS_TEMPLATE_MACRO_BASE(ClassName, TemplateMacro, Template, ...) \
	TemplateMacro(Template)                               \
	class ClassName;                                        \
	CLASS_MACRO_COMMON(ClassName, __VA_ARGS__)              \
	TemplateMacro(Template)                               \
	class ClassName : public ClassName##_PARENT

#define CLASS(ClassName, ...) CLASS_TEMPLATE_MACRO_BASE(ClassName, NONE, NONE, __VA_ARGS__)
#define CLASS_NESTED(ClassName, ...) CLASS_TEMPLATE_MACRO_BASE(ClassName, NONE, NONE, __VA_ARGS__) // needed so generated code script can ignore nested classes
#define CLASS_TEMPLATE(ClassName, Template, ...) CLASS_TEMPLATE_MACRO_BASE(ClassName, TEMPLATE_MACRO, Template, __VA_ARGS__)

#define SINGLETON(...) \
public Singleton<__VA_ARGS__>

#define GENERATE_ID_STATIC(...)                                  \
	STC ClassId getClassIdStatic()                            \
	{                                                            \
		STC ClassId classId = Hash::hashString(#__VA_ARGS__); \
		return classId;                                          \
	};

#define GENERATE_ID_VIRTUAL(...)                         \
	ClassId getClassId() CNS OVR                  \
	{                                                    \
		return __VA_ARGS__##_PARENT::getClassIdStatic(); \
	};

#define GENERATE_NAME_STATIC(...)                                 \
	STC SStr getClassNameStatic()                       \
	{                                                             \
		STC SStr className = SStr(#__VA_ARGS__); \
		return className;                                         \
	};

#define GENERATE_NAME_VIRTUAL(...)                         \
	SStr getClassName() CNS OVR              \
	{                                                      \
		return __VA_ARGS__##_PARENT::getClassNameStatic(); \
	};

#define GENERATE_ATTRIBUTES_NAMES_STATIC(...)                                        \
	STC SLst<AttributeBase> &__getClassAttributesNamesStatic()          \
	{                                                                          \
		STC SLst<AttributeBase> attributesNames = {};\
		return attributesNames;                                                   \
	};

#define GENERATE_ATTRIBUTES_NAMES_STATIC_CONST(...)                                        \
	STC CNS SLst<AttributeBase> &getClassAttributesNamesStatic()          \
	{                                                                          \
		return __getClassAttributesNamesStatic();                               \
	};

#define GENERATE_DYNAMIC_DESTRUCTOR_VIRTUAL(...) \
	VIR void dynamicDestructor() OVR { this->~__VA_ARGS__(); };

#define GENERATE_METADATA(...)          \
protected:\
	GENERATE_ATTRIBUTES_NAMES_STATIC(__VA_ARGS__);  \
public:                                 \
	GENERATE_NAME_STATIC(__VA_ARGS__);  \
	GENERATE_NAME_VIRTUAL(__VA_ARGS__); \
	GENERATE_ID_STATIC(__VA_ARGS__);    \
	GENERATE_ID_VIRTUAL(__VA_ARGS__);   \
	GENERATE_ATTRIBUTES_NAMES_STATIC_CONST(__VA_ARGS__);  \
                                        \
private:

// --------------------------------------------------------
// MEMBERS, GETTERS AND SETTERS
// --------------------------------------------------------

#define OVR override
#define VIR virtual
#define STC static
#define CNS const

#define GETTER_TYPE(Var)                                            \
	COND_TYPE(                                                      \
		IS_POINTER(decltype(Var)),                                  \
		ADD_CONST(decltype(Var)),                                   \
		COND_TYPE(                                                  \
			IS_ARITHMETIC(decltype(Var)) || IS_ENUM(decltype(Var)), \
			REMOVE_REF(decltype(Var)),                              \
			decltype(Var)))

#define SETTER_TYPE(Var) \
	COND_TYPE(                                                      \
		IS_POINTER(decltype(Var)),                                  \
		ADD_CONST(decltype(Var)),                                   \
		COND_TYPE(                                                  \
			IS_ARITHMETIC(decltype(Var)) || IS_ENUM(decltype(Var)), \
			REMOVE_REF(decltype(Var)),                              \
			ADD_REFERENCE(ADD_CONST(decltype(Var)))))

#define GET(BaseName)        \
	GETTER_TYPE(m##BaseName) get##BaseName() CNS { return m##BaseName; };

#define GETREF(BaseName) \
	ADD_REFERENCE(GETTER_TYPE(m##BaseName)) get##BaseName() { return m##BaseName; };

#define GETREF_CONST(BaseName) \
	ADD_REFERENCE(ADD_CONST(GETTER_TYPE(m##BaseName))) get##BaseName() CNS { return m##BaseName; };

#define SET(BaseName) \
	void set##BaseName(SETTER_TYPE(m##BaseName) new##BaseName) { m##BaseName = new##BaseName; };

#define GET_SET(BaseName) GET(BaseName) SET(BaseName)
#define GETREF_SET(BaseName) GETREF(BaseName) SET(BaseName)
#define GETREF_CONST_SET(BaseName) GETREF_CONST(BaseName) SET(BaseName)

#define MEMBER_BASE(BaseName, ...) \
	__VA_ARGS__ m##BaseName = {};

#define MEMBER(BaseName, AccessorMacroName, Visibility, ...) \
	Visibility:                                              \
	MEMBER_BASE(BaseName, __VA_ARGS__)                       \
	public : AccessorMacroName(BaseName)                     \
	private : \
	inline STC AttributeRegisterStatic __attributeRegisterStatic##BaseName = AttributeRegisterStatic(getClassNameStatic(), #BaseName, __getClassAttributesNamesStatic()); \
	AttributeRegister __attributeRegister##BaseName = AttributeRegister(#BaseName, (void*) &m##BaseName, this); \
	Visibility:
	
#define PUB(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, public, __VA_ARGS__)
#define PRO(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, protected, __VA_ARGS__)
#define PRI(BaseName, AccessorMacroName, ...) MEMBER(BaseName, AccessorMacroName, private, __VA_ARGS__)

// --------------------------------------------------------
// COPY
// --------------------------------------------------------

#define COPY(...)                                                                        \
	VIR void copy(CNS ObjectBase *other) OVR                                  \
	{                                                                                    \
		if (this != other)                                                               \
		{                                                                                \
			if (CNS __VA_ARGS__ *otherCast = dynamic_cast<CNS __VA_ARGS__ *>(other)) \
			{                                                                            \
				specificCopy(otherCast);                                                 \
			}                                                                            \
		}                                                                                \
	}                                                                                    \
                                                                                         \
	void specificCopy(CNS __VA_ARGS__ *other)

#define DO_COPY(BaseName) m##BaseName = other->m##BaseName;

// --------------------------------------------------------
// SERIALIZATION
// --------------------------------------------------------

// SERIALIZE

#define SERIALIZE()\
VIR void serialize(JSON &json) CNS OVR

#define SUPER_SERIALIZE()\
Super::serialize(json);

// This macro must be used in .cpp
#define SERIALIZE_IMPL(...)\
void __VA_ARGS__::serialize(JSON &json) CNS

#define DO_SERIALIZE(Name, Var)\
json[Name] = SerializationUtils::serializeTemplated<decltype(Var)>(Var);

#define DO_SERIALIZE_IF(Condition, Name, Var)\
if((Condition))\
{\
	DO_SERIALIZE(Name, Var)\
}

#define DO_SERIALIZE_LIST_ELEMENT(Name, Var)\
json[Name].push_back(SerializationUtils::serializeTemplated<decltype(Var)>(Var));

#define DO_SERIALIZE_LIST(Name, Var)\
FOR_LIST(__it, Var)\
{\
	DO_SERIALIZE_LIST_ELEMENT(Name, (*__it))\
}

#define DO_SERIALIZE_LIST_IF(Name, Var, ConditionLambda)\
FOR_LIST(__it, Var)\
{\
	if((ConditionLambda(*__it)))\
	{\
		DO_SERIALIZE_LIST_ELEMENT(Name, (*__it))\
	}\
}

// DESERIALIZE

#define DESERIALIZE()\
VIR void deserialize(CNS JSON &json) OVR

#define SUPER_DESERIALIZE()\
Super::deserialize(json);

#define DESERIALIZE_IMPL(...)\
void __VA_ARGS__::deserialize(CNS JSON &json)

#define DO_DESERIALIZE(Name, Var)\
SerializationUtils::deserializeTemplated<decltype(Var)>(Var, json[Name]);

#define DO_DESERIALIZE_LIST(Name, Var, ConstructionLambda)\
FOR_LIST(__it, json[Name])\
{\
	auto object = ConstructionLambda(*__it);\
	SerializationUtils::deserializeTemplated<decltype(object)>(object, (*__it));\
	(Var).push_back(object);\
}

// --------------------------------------------------------
// FOR LOOPS
// --------------------------------------------------------

#define FOR_MAP(it, map) \
	for (auto it = (map).begin(); it != (map).end(); ++it)

#define FOR_RANGE(it, start, size) for (i32 it = start; it < (i32)(size); ++it)
#define FOR_RANGE_COND(it, start, size, condition) for (i32 it = start; (it < (i32)(size)) && (condition); ++it)

#define FOR_ARRAY(it, array) FOR_RANGE(it, 0, (array).size())
#define FOR_ARRAY_COND(it, array, condition) FOR_RANGE_COND(it, 0, (array).size(), (condition))

#define FOR_LIST(it, list) for (auto it = (list).begin(); it != (list).end(); ++it)
#define FOR_LIST_COND(it, list, condition) for (auto it = (list).begin(); (it != (list).end()) && (condition); ++it)

// --------------------------------------------------------
// PREPROCESOR FOR EACH
// --------------------------------------------------------

#include "generated-code/for_each.generated.hpp"

// --------------------------------------------------------
// STD CONTAINERS
// --------------------------------------------------------

#define MAP_CONTAINS(map, key) ((map).find(key) != (map).end())
#define MAP_INSERT(map, key, value) (map).insert_or_assign((key), (value));

#define MAP_DELETE_CONTENT(map)           \
	FOR_MAP(_internal_it, map)            \
	{                                     \
		if (_internal_it->second)         \
			DELETE(_internal_it->second); \
	}                                     \
	(map).clear();

#define LIST_DELETE_CONTENT(list)                       \
FOR_LIST(_internal_it, list) { DELETE(*_internal_it); } \
	(list).clear();
