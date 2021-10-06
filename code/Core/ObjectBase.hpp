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

private:

	ObjectId mObjectId = 0;

	SMap(SStr, AttributeBase) mAttributes; // runtime attributes

protected:
	inline static ObjectId smObjectIdCounter = 0;

private:

	GENERATE_ATTRIBUTES_NAMES_STATIC(ObjectBase)

public:
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

	VIR ~ObjectBase(){};

	ObjectId getObjectId()
	{
		return mObjectId;
	}

	VIR ClassId getClassId() const
	{
		return ObjectBase::getClassIdStatic();
	}

	VIR SStr getClassName() const
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

	VIR void copy(const ObjectBase *other)
	{
	}

	ObjectBase &operator=(const ObjectBase &other)
	{
		copy(&other);
		return *this;
	}
};
