#pragma once

#include "Core/Hash.hpp"
#include <type_traits>
#include <iostream>

#include "Core/Macros.hpp"

namespace DE {

/*!
 \brief Base class.
 */
class ObjectBase : public Hash {

public:
	static String getClassNameStatic() {
		static String className = "ObjectBase";
		return className;
	};

	static ClassId getClassIdStatic() {
		return 0;
	};

	ObjectBase() = default;
	virtual ~ObjectBase() = default;
	virtual void dynamicDestructor(){ this->~ObjectBase(); };

	virtual ClassId getClassId() const {
		return ObjectBase::getClassIdStatic();
	};

	virtual String getClassName() const {
		return ObjectBase::getClassNameStatic();
	};

	template<class T>
	bool isDerivedClass() const {
		return dynamic_cast<const T*>(this) != nullptr;
	}

	template<class T>
	T* castTo() {
		return dynamic_cast<T*>(this);
	}

	template<class T>
	bool isSameClass() const {
		return isDerivedClass<ObjectBase>() && (this->getClassId() == T::getClassIdStatic());
	}

	bool hasSameClass(ObjectBase* object) const {
		return this->getClassId() == object->getClassId();
	}
};
}

