#pragma once

#include "Core/Basic.hpp"
#include "Memory/Memory.hpp"

namespace DE {

template<class T>
class Singleton {

protected:
	static T* mInstance;

public:
	Singleton() = default;

	~Singleton() = default;

	static bool existsInstance() {
		return mInstance != nullptr;
	}	

	static T* getInstance() {
		if (!existsInstance()) {
			mInstance = DE_NEW<T>();
		}

		return mInstance;
	};

};

template<class T>
T* Singleton<T>::mInstance = nullptr;

};
/* namespace DE */

