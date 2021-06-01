#pragma once

template<class T>
class Singleton {

protected:
	static T* mInstance;

public:
	Singleton() = default;

	~Singleton() = default;

	static bool existsInstance(){
		return mInstance != nullptr;
	}	

	static T* getInstance(){
		if (!existsInstance()){
			mInstance = NEW(T);
		}

		return mInstance;
	}

	static void deleteInstance(){
		if(mInstance){
			DELETE(mInstance);
			mInstance = nullptr;
		}
	}	
};

template<class T>
T* Singleton<T>::mInstance = nullptr;