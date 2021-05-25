#pragma once

#include "Core/BasicTypes.hpp"

#include "Core/Macros.hpp"
#include "Log/Log.hpp"

namespace Hash {
	static u32 hashString(std::string key){
		std::hash<std::string> hash_fn;
		u32 hashString = hash_fn(key);
		return hashString;
	}
}

/*!
 \brief Base class.
 */
class ObjectBase {

protected:

	static ObjectId smObjectIdCounter;

	PRI(ObjectId, GET, ObjectId);

public:

	static std::string getClassNameStatic(){
		static std::string className = "ObjectBase";
		return className;
	}

	static ClassId getClassIdStatic(){
		static ClassId classId = Hash::hashString("ObjectBase");
		return classId;
	}

	ObjectBase(){ if(mObjectId == 0){ mObjectId = smObjectIdCounter++; } };
	virtual ~ObjectBase(){ };

	virtual ClassId getClassId() const {
		return ObjectBase::getClassIdStatic();
	}

	virtual std::string getClassName() const {
		return ObjectBase::getClassNameStatic();
	}

	template<class T>
	bool isDerivedClass() const {
		return dynamic_cast<const T*>(this) != nullptr;
	}

	template<class T>
	bool isSameClass() const {
		return isDerivedClass<ObjectBase>() && (this->getClassId() == T::getClassIdStatic());
	}

	bool hasSameClass(ObjectBase* object) const {
		return this->getClassId() == object->getClassId();
	}

	// Assignment

	virtual void copy(const ObjectBase *other){
		
	}

	ObjectBase& operator= (const ObjectBase &other){
		copy(&other);
		return *this;
	}
};
