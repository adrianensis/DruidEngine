#pragma once

#include "Core/ObjectBase.hpp"
#include <functional>

CLASS_TEMPLATE(Functor, T, ObjectBase)
{
	PRO(Callback, NONE, T)

public:
	virtual void execute() = 0;

	void setCallback(T newCallback)
	{
		mCallback = newCallback;
	};

	// NOTE : Override in children if needed!
	COPY(Functor<T>)
	{
		DO_COPY(Callback)
	}

	/*bool operator== (const Functor& functor) const{
	   return this == &functor;
	}*/
};

CLASS(FunctorVoid, Functor<std::function<void()>>){
	public :
		virtual void execute() override{
			if (mCallback)
				mCallback();
}
}
;
