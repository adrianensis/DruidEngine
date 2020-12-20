#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.hpp"
#include <string>
#include <iostream>

namespace DE {

// --------------------------------------------------------
// METADATA MACROS
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

#define DE_CLASS(Class, ParentClass) Class(); virtual ~Class() override; DE_GENERATE_METADATA(Class, ParentClass)

// --------------------------------------------------------
// TEMPLATES
// --------------------------------------------------------

#define DE_CHECK_ARITHMETIC_TYPENAME(X, TemplateClass, result)\
		if(std::is_same<X, TemplateClass>::value){\
			result = #X;\
		}\

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
// GETTERS AND SETTERS
// --------------------------------------------------------

#define DE_GET(Class, BaseName) Class get ## BaseName() const { return m ## BaseName; }
#define DE_SET(Class, BaseName) void set ## BaseName (Class new ## BaseName){ m ## BaseName = new ## BaseName; }
#define DE_GET_SET(Class, BaseName) DE_GET(Class, BaseName) DE_SET(Class, BaseName)

// ---------------------------------------------------------------------------

/*!
 \brief Base class for Druid Engine.
 */
class DE_Class: public Hash {

public:

	template<class TT>
	static std::string calculateTemplateName() {
		std::string templateName;
		using T_noPointer = typename std::remove_pointer<TT>::type;
		if constexpr (std::is_base_of<DE_Class, T_noPointer>::value){ // constexpr avoids compilation-time errors when, for example TT == float
			templateName = std::remove_pointer<TT>::type::getClassNameStatic();
		}else if constexpr (std::is_arithmetic<TT>::value){

			DE_CHECK_ARITHMETIC_TYPENAME(f32, TT, templateName)
			DE_CHECK_ARITHMETIC_TYPENAME(f64, TT, templateName)

			DE_CHECK_ARITHMETIC_TYPENAME(c8, TT, templateName)

			DE_CHECK_ARITHMETIC_TYPENAME(i8, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(i16, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(i32, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(i64, TT, templateName);

			DE_CHECK_ARITHMETIC_TYPENAME(byte, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(u8, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(u16, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(u32, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(u64, TT, templateName);

		}else if constexpr (std::is_same<std::string, TT>::value){
			templateName = "string";
		}else if constexpr (std::is_pointer<TT>::value){ // Non DE_Class pointers
			templateName = typeid(TT).name();
		}else {
			templateName = typeid(TT).name();
		}

		return "["+templateName+"]";
	};

	static std::string getClassNameStatic() {
		static std::string className = "DE_Class";
		return className;
	};

	static std::string getTemplateNameStatic() {
		static std::string templateName = "";
		return templateName;
	};

	static ClassId getClassIdStatic() {
		return 0;
	}

	DE_Class() = default;
	virtual ~DE_Class() = default;

	virtual void dynamicDestructor() { }

	virtual ClassId getClassId() const {
		return DE_Class::getClassIdStatic();
	};

	virtual ClassId getParentClassId() const {
		return DE_Class::getClassIdStatic();
	};

	virtual std::string getClassName() const {
		return DE_Class::getClassNameStatic();
	};

	virtual std::string getTemplateName() const {
		return DE_Class::getTemplateNameStatic();
	};
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif //DE_CLASS_H
