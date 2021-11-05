#pragma once

#include "Core/Core.hpp"
#include "Assert/Assert.hpp"

template <class T>
class Functor: public ObjectBase
{
	GENERATE_METADATA(Functor<T>)
	PRO T mCallback; SET(Callback)

PUB
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

class FunctorVoid: public Functor<std::function<void()>>
{
	GENERATE_METADATA(FunctorVoid)
PUB	
	virtual void execute() override
	{
		ASSERT_MSG(mCallback != nullptr, "Callback is null.");

		if (mCallback)
		{
			mCallback();
		}
	}
};
