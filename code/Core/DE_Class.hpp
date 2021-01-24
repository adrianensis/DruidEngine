#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Core/Hash.hpp"
#include <string>
#include <iostream>

namespace DE {

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

			DE_CHECK_ARITHMETIC_TYPENAME(f32, TT, templateName);
			DE_CHECK_ARITHMETIC_TYPENAME(f64, TT, templateName);

			DE_CHECK_ARITHMETIC_TYPENAME(c8, TT, templateName);

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
			templateName = typeid(TT).name(); // default
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

	virtual std::string getClassName() const {
		return DE_Class::getClassNameStatic();
	};

	virtual std::string getTemplateName() const {
		return DE_Class::getTemplateNameStatic();
	};

	template<class T>
	bool isDerived() const {
		return dynamic_cast<const T*>(this) != nullptr;
	}
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif //DE_CLASS_H
