#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/ObjectBase.hpp"

template <class T, typename = std::enable_if_t<std::is_base_of<ObjectBase, T>::value> >
class ProxyObject
{
	PRO T* mObject = nullptr;

PRI 
	virtual void onDestroy() { mObject = nullptr; }

PUB

	void init(T* object)
	{
		mObject = object;

		mObject->setProxyCallback([this]()
		{
			onDestroy();
		});
	}

	void disconnectFromOwner()
	{
		if(mObject)
		{
			mObject->setProxyCallback([](){});
		}
	}

    bool isValid() const { return mObject != nullptr; }
	T* getObject() const { return mObject; }
};