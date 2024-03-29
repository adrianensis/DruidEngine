#pragma once

#include "Core/ObjectBase.hpp"
#include "Assert/Assert.hpp"
#include <functional>

CLASS_TEMPLATE(Functor, T, ObjectBase)
{
	PRO(Callback, SET, T)

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

CLASS(FunctorVoid, Functor<std::function<void()>>){
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
