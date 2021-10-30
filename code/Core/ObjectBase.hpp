#pragma once

#include "Core/BasicTypes.hpp"

#include "Core/Macros.hpp"
#include "Log/Log.hpp"
#include "Core/Serialization.hpp"
#include "Core/AttributeBase.hpp"

namespace Hash
{
	static u32 hashString(SStr key)
	{
		std::hash<SStr> hash_fn;
		u32 hashString = hash_fn(key);
		return hashString;
	}
}

class ObjectBase: public ISerializable
{
	friend AttributeRegister;

PRI

	ObjectId mObjectId = 0;

	SMap(SStr, AttributeBase) mAttributes; // runtime attributes

PRO
	inline static ObjectId smObjectIdCounter = 0;

PRI

	GENERATE_ATTRIBUTES_NAMES_STATIC(ObjectBase)

PUB
	static SStr getClassNameStatic()
	{
		static SStr className = "ObjectBase";
		return className;
	}

	static ClassId getClassIdStatic()
	{
		static ClassId classId = Hash::hashString("ObjectBase");
		return classId;
	}

	GENERATE_ATTRIBUTES_NAMES_STATIC_CONST(ObjectBase)

	ObjectBase()
	{
		if (mObjectId == 0)
		{
			mObjectId = smObjectIdCounter++;
		}
	};

	virtual ~ObjectBase(){};

	ObjectId getObjectId()
	{
		return mObjectId;
	}

	virtual ClassId getClassId() const
	{
		return ObjectBase::getClassIdStatic();
	}

	virtual SStr getClassName() const
	{
		return ObjectBase::getClassNameStatic();
	}

	const SMap(SStr, AttributeBase) &getAttributes()
	{
		return mAttributes;
	}

	template <class T>
	bool isDerivedClass() const
	{
		return dynamic_cast<const T *>(this) != nullptr;
	}

	template <class T>
	bool isSameClass() const
	{
		return isDerivedClass<ObjectBase>() && (this->getClassId() == T::getClassIdStatic());
	}

	bool hasSameClass(ObjectBase *object) const
	{
		return this->getClassId() == object->getClassId();
	}

	// Assignment

	virtual void copy(const ObjectBase *other)
	{
	}

	ObjectBase &operator=(const ObjectBase &other)
	{
		copy(&other);
		return *this;
	}
};
