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
	inline static std::string smClassName = "ObjectBase";
	inline static ClassId smClassId = Hash::hashString(smClassName);


PRO
	inline static ObjectId smObjectIdCounter = 0;

PUB

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

	static std::string getClassNameStatic()
	{
		return smClassName;
	}

	static ClassId getClassIdStatic()
	{
		return smClassId;
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
