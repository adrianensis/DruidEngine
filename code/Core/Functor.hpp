#pragma once

#include "Core/Core.hpp"
#include "Assert/Assert.hpp"

CLASS_TEMPLATE(Functor, T, ObjectBase)
{
	PRO(T, Callback, SET)

public:
	virtual void execute() = 0;

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
	virtual void execute() override
	{
		ASSERT_MSG(mCallback != nullptr, "Callback is null.");

		if (mCallback)
		{
			mCallback();
		}
	}
};
