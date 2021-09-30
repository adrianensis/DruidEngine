#pragma once

#include "Core/BasicTypes.hpp"

#include "Core/Macros.hpp"
#include "Log/Log.hpp"
#include "Core/Serialization.hpp"
#include "Core/AttributeBase.hpp"

namespace Hash
{
	STC u32 hashString(SStr key)
	{
		std::hash<SStr> hash_fn;
		u32 hashString = hash_fn(key);
		return hashString;
	}
}

class ObjectBase: public ISerializable
{
	friend AttributeRegister;

private:

	ObjectId mObjectId = 0;

	SMap(SStr, AttributeBase) mAttributes; // runtime attributes

protected:
	inline STC ObjectId smObjectIdCounter = 0;

private:

	GENERATE_ATTRIBUTES_NAMES_STATIC(ObjectBase)

public:
	STC SStr getClassNameStatic()
	{
		STC SStr className = "ObjectBase";
		return className;
	}

	STC ClassId getClassIdStatic()
	{
		STC ClassId classId = Hash::hashString("ObjectBase");
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

	VIR ~ObjectBase(){};

	ObjectId getObjectId()
	{
		return mObjectId;
	}

	VIR ClassId getClassId() CNS
	{
		return ObjectBase::getClassIdStatic();
	}

	VIR SStr getClassName() CNS
	{
		return ObjectBase::getClassNameStatic();
	}

	CNS SMap(SStr, AttributeBase) &getAttributes()
	{
		return mAttributes;
	}

	template <class T>
	bool isDerivedClass() CNS
	{
		return dynamic_cast<CNS T *>(this) != nullptr;
	}

	template <class T>
	bool isSameClass() CNS
	{
		return isDerivedClass<ObjectBase>() && (this->getClassId() == T::getClassIdStatic());
	}

	bool hasSameClass(ObjectBase *object) CNS
	{
		return this->getClassId() == object->getClassId();
	}

	// Assignment

	VIR void copy(CNS ObjectBase *other)
	{
	}

	ObjectBase &operator=(CNS ObjectBase &other)
	{
		copy(&other);
		return *this;
	}
};
