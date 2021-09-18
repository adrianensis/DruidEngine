#pragma once

#include "Core/Memory.hpp"

template <class T>
class Singleton
{
protected:
	STC T *mInstance;

public:
	Singleton() = default;

	~Singleton() = default;

	STC bool existsInstance()
	{
		return mInstance != nullptr;
	}

	STC T *getInstance()
	{
		if (!existsInstance())
		{
			mInstance = Memory::newObject<T>();
		}

		return mInstance;
	}

	STC void deleteInstance()
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