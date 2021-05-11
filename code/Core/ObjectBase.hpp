#pragma once

#include "Core/Hash.hpp"
#include <type_traits>
#include <iostream>
#include <list>
#include <any>

#include "Core/Macros.hpp"

/*!
 \brief Base class.
 */
class ObjectBase {

protected:

	static ObjectId smObjectIdCounter;

	PRI(ObjectId, GET, ObjectId);

public:

	static std::string getClassNameStatic() {
		static std::string className = "ObjectBase";
		return className;
	};

	static ClassId getClassIdStatic() {
		static ClassId classId = Hash::hashString("ObjectBase");
		return classId;
	};

	ObjectBase() { if(mObjectId == 0) { mObjectId = smObjectIdCounter++; } };
	virtual ~ObjectBase() = default;

	virtual ClassId getClassId() const {
		return ObjectBase::getClassIdStatic();
	};

	virtual std::string getClassName() const {
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

	ObjectBase& operator= (const ObjectBase &other) {
		return *this;
	}
};
