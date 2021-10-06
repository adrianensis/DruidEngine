#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/ObjectBase.hpp"
#include "Core/Assert/Assert.hpp"


class Memory
{
public:
	static SMap(SStr, i32) mAllocationsCounter;

	static void init();
	static void terminate();

	template <class T>
	static T *newObject()
	{
		T *object = new T;

		SStr className;

		if constexpr (std::is_base_of<ObjectBase, T>::value)
		{
			className = object->getClassName();
		}
		else
		{
			className = typeid(T).name();
		}

		if (MAP_CONTAINS(mAllocationsCounter, className))
		{
			MAP_INSERT(mAllocationsCounter, className, mAllocationsCounter[className] + 1);
		}
		else
		{
			MAP_INSERT(mAllocationsCounter, className, 1);
		}

		return object;
	}

	template <class T>
	static void deleteObject(T * pointer)
	{
		ASSERT_MSG(pointer != nullptr, "pointer is nullptr");

		SStr className;
		if constexpr (std::is_base_of<ObjectBase, T>::value)
		{
			className = pointer->getClassName();
		}
		else
		{
			className = typeid(T).name();
		}

		if (MAP_CONTAINS(mAllocationsCounter, className))
		{
			MAP_INSERT(mAllocationsCounter, className, mAllocationsCounter[className] - 1);
		}

		delete pointer;
	}
};