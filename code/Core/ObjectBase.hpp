#pragma once

#include "Core/BasicTypes.hpp"

#include "Core/Macros.hpp"
#include "Core/Serialization.hpp"
#include "Core/Proxy/ProxyOwner.hpp"

namespace Hash
{
	static u64 hashString(std::string key)
	{
		std::hash<std::string> hash_fn;
		u64 hashString = hash_fn(key);
		return hashString;
	}
}

class ObjectBase: public ISerializable, public ProxyOwner
{
PRI

	ObjectId mObjectId = 0;

PRO
	inline static ObjectId smObjectIdCounter = 0;

PUB
	static std::string getClassNameStatic()
	{
		static std::string className = "ObjectBase";
		return className;
	}

	static ClassId getClassIdStatic()
	{
		static ClassId classId = Hash::hashString("ObjectBase");
		return classId;
	}

	ObjectBase()
	{
		if (mObjectId == 0)
		{
			mObjectId = smObjectIdCounter++;
		}
	};

	virtual ~ObjectBase()
	{
		notifyProxy();
	};

	ObjectId getObjectId()
	{
		return mObjectId;
	}

	virtual ClassId getClassId() const
	{
		return ObjectBase::getClassIdStatic();
	}

	virtual std::string getClassName() const
	{
		return ObjectBase::getClassNameStatic();
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
