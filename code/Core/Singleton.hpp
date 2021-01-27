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

	static T* getInstance() {
		if (!mInstance) {
			mInstance = DE_NEW<T>();
		}

		return mInstance;
	};

};

template<class T>
T* Singleton<T>::mInstance = nullptr;

};
/* namespace DE */

