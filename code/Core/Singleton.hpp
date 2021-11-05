#pragma once

#include "Core/Memory.hpp"

template <class T>
class Singleton
{
PRO
	static T *mInstance;

PUB
	Singleton() = default;

	~Singleton() = default;

	static bool existsInstance()
	{
		return mInstance != nullptr;
	}

	static T &getInstance()
	{
		if (!existsInstance())
		{
			mInstance = Memory::newObject<T>();
		}

		return *mInstance;
	}

	static void deleteInstance()
	{
		if (mInstance)
		{
			Memory::deleteObject<T>(mInstance);
			mInstance = nullptr;
		}
	}
};

template <class T>
T *Singleton<T>::mInstance = nullptr;