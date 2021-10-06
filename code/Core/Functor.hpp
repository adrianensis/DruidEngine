#pragma once

#include "Core/Core.hpp"
#include "Assert/Assert.hpp"

CLASS_TEMPLATE(Functor, T, ObjectBase)
{
	PRO(T, Callback, SET)

public:
	VIR void execute() = 0;

	// NOTE : Override in children if needed!
	COPY(Functor<T>)
	{
		DO_COPY(Callback)
	}

	/*bool operator== (const Functor& functor) const{
	   return this == &functor;
	}*/
};

CLASS(FunctorVoid, Functor<SFun(void()>)){
public:	
	VIR void execute() OVR
	{
		ASSERT_MSG(mCallback != nullptr, "Callback is null.");

		if (mCallback)
		{
			mCallback();
		}
	}
};
