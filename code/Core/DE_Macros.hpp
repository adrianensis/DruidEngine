#ifndef DE_MACROS_H
#define DE_MACROS_H

#include <cstdint> // std::uintptr_t
#include <type_traits>
#include <string>

namespace DE {

// --------------------------------------------------------
// CLASS - METADATA MACROS
// --------------------------------------------------------

#define DE_GENERATE_ID_STATIC(Class) static ClassId getClassIdStatic(){static ClassId classId = Hash::hash(#Class); return classId;}
#define DE_GENERATE_PARENT_ID_STATIC(ParentClass) static ClassId getParentClassIdStatic(){ return ParentClass::getClassIdStatic(); }

#define DE_GENERATE_ID_VIRTUAL(Class) ClassId getClassId() const override { return Class::getClassIdStatic(); };
#define DE_GENERATE_PARENT_ID_VIRTUAL(ParentClass) ClassId getParentClassId() const override { return ParentClass::getClassIdStatic(); };

#define DE_GENERATE_NAME_STATIC(Class) static std::string getClassNameStatic(){static std::string className = std::string(#Class + Class::getTemplateNameStatic()); return className;}
#define DE_GENERATE_NAME_VIRTUAL(Class) std::string getClassName() const override { return Class::getClassNameStatic(); };

#define DE_GENERATE_METADATA(Class, ParentClass)\
		DE_GENERATE_NAME_STATIC(Class);\
		DE_GENERATE_NAME_VIRTUAL(Class);\
		DE_GENERATE_ID_STATIC(Class);\
		DE_GENERATE_PARENT_ID_STATIC(ParentClass);\
		DE_GENERATE_ID_VIRTUAL(Class);\
		DE_GENERATE_PARENT_ID_VIRTUAL(ParentClass);

#define DE_CLASS(Class, ParentClass)\
		Class();\
		virtual ~Class() override;\
		DE_GENERATE_METADATA(Class, ParentClass)\

// --------------------------------------------------------
// TEMPLATES
// --------------------------------------------------------

#define DE_CHECK_ARITHMETIC_TYPENAME(X, TemplateClass, result)\
		if(std::is_same<X, TemplateClass>::value){\
			result = #X;\
		}

#define DE_GENERATE_TEMPLATE_STATIC(TemplateClass)\
		static std::string getTemplateNameStatic(){\
			static std::string templateName=DE_Class::calculateTemplateName<TemplateClass>();\
			return templateName;\
		};

#define DE_GENERATE_TEMPLATE_VIRTUAL(Class) virtual std::string getTemplateName() const override { return Class::getTemplateNameStatic(); };

#define DE_CLASS_TEMPLATE_COMMON(Class, ParentClass)\
		DE_GENERATE_TEMPLATE_VIRTUAL(Class);\
		DE_GENERATE_METADATA(Class, ParentClass);

#define DE_CLASS_TEMPLATE(Class, ParentClass, TemplateClass)\
		DE_GENERATE_TEMPLATE_STATIC(TemplateClass);\
		DE_CLASS_TEMPLATE_COMMON(Class, ParentClass)

#define DE_GENERATE_TEMPLATE_STATIC2(TemplateClass1, TemplateClass2)\
		static std::string getTemplateNameStatic(){\
			static std::string templateName=DE_Class::calculateTemplateName<TemplateClass1>() +\
			DE_Class::calculateTemplateName<TemplateClass2>();\
			return templateName;\
		};

#define DE_CLASS_TEMPLATE2(Class, ParentClass, TemplateClass1, TemplateClass2)\
		DE_GENERATE_TEMPLATE_STATIC2(TemplateClass1, TemplateClass2);\
		DE_CLASS_TEMPLATE_COMMON(Class, ParentClass)

// --------------------------------------------------------
// MEMBERS, GETTERS AND SETTERS
// --------------------------------------------------------

#define DE_IS_POINTER(Class) std::is_pointer<std::remove_reference<Class>::type>::value
#define DE_IS_ARITHMETIC(Class) std::is_arithmetic<std::remove_reference<Class>::type>::value

#define DE_MEMBER_BASE(BaseName, Class) Class m ## BaseName = {};

#define DE_PROTECTED_MEMBER(BaseName, Class) protected: DE_MEMBER_BASE(BaseName, Class) private:
#define DE_PUBLIC_MEMBER(BaseName, Class) public: DE_MEMBER_BASE(BaseName, Class) public:

#define DE_M(BaseName, Class) DE_PROTECTED_MEMBER(BaseName, Class)
#define DE_PUBLIC_M(BaseName, Class) DE_PUBLIC_MEMBER(BaseName, Class)

#define DE_COND_TYPE(Bool, T1, T2) std::conditional<Bool, T1, T2>::type

#define DE_GETTER_TYPE(Var)\
	DE_COND_TYPE(\
		DE_IS_POINTER(decltype(Var)),\
		std::add_const<std::remove_reference<decltype(Var)>::type>::type,\
		DE_COND_TYPE(\
			DE_IS_ARITHMETIC(decltype(Var)),\
			std::remove_reference<decltype(Var)>::type,\
			std::add_const<decltype(Var)>::type\
		)\
	)

#define DE_SETTER_TYPE(Var) DE_GETTER_TYPE(Var) // NOTE: It's the same as the getter.

#define DE_GET(BaseName)\
	public: DE_GETTER_TYPE(m ## BaseName) get ## BaseName() const { return m ## BaseName; };

#define DE_SET(BaseName)\
	public: void set ## BaseName (DE_SETTER_TYPE(m ## BaseName) new ## BaseName){ m ## BaseName = new ## BaseName; };

#define DE_GET_SET(BaseName, Class) DE_GET(BaseName) DE_SET(BaseName)

#define DE_M_GET(BaseName, Class)\
	DE_M(BaseName, Class) DE_GET(BaseName)

#define DE_M_SET(BaseName, Class)\
	DE_M(BaseName, Class) DE_SET(BaseName)

#define DE_M_GET_SET(BaseName, Class) DE_M(BaseName, Class) DE_GET_SET(BaseName, Class)

// --------------------------------------------------------
// FOR LOOPS
// --------------------------------------------------------

#define FOR_RANGE(it, start, size) for (i32 it = start; it < size; ++it)
#define FOR_RANGE_COND(it, start, size, condition) for (i32 it = start; it < size && condition; ++it)

#define FOR_ARRAY(it, array) FOR_RANGE(it, 0, array->getLength())
#define FOR_ARRAY_COND(it, array, condition) for (u32 it = 0; it < array->getLength() && condition; ++it)

#define FOR_LIST(it, list) for (auto it = list->getIterator(); !it.isNull(); it.next())
#define FOR_LIST_COND(it, list, cond) for (auto it = list->getIterator(); (!it.isNull()) && cond; it.next())

}

#endif /* DE_MACROS_H */
