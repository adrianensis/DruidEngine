#ifndef DE_FUNCTOR_H_
#define DE_FUNCTOR_H_

#include "DE_Class.hpp"
#include <functional>

namespace DE {

template <class T>
class Functor: public DE_Class {

protected:

	T mCallback;

public:

	DE_CLASS_TEMPLATE(Functor<T>, DE_Class, T);

	Functor() : DE_Class(){};
	virtual ~Functor() override {};

	virtual void execute() = 0;

	void setCallback(T newCallback) {
		mCallback = newCallback;
	};

	// NOTE : Override in children!
	Functor& operator= (const Functor &functor) {
		// self-assignment guard
		if (this == &functor)
			return *this;

		// do the copy
		mCallback = functor.mCallback;

		return *this;
	}

	bool operator== (const Functor& functor) const{
	    return this == &functor;
	}
};

class FunctorVoid : public Functor<std::function<void()>> {
public:

	DE_GENERATE_METADATA(FunctorVoid, Functor<std::function<void()>>);

	FunctorVoid() : Functor<std::function<void()>>(){};
	virtual ~FunctorVoid() override {};

	virtual void execute() override {
		if (mCallback)
			mCallback();
	}

};

} /* namespace DE */

#endif /* DE_FUNCTOR_H_ */
