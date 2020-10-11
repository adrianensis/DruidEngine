#ifndef DE_FUNCTOR_H_
#define DE_FUNCTOR_H_

#include "DE_Class.hpp"
#include <functional>

namespace DE {

template <class T>
class Functor {

protected:

	T mCallback;

public:
	Functor() = default;
	virtual ~Functor() = default;

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
	FunctorVoid() = default;
	~FunctorVoid() override = default;

	virtual void execute() override {
		if (mCallback)
			mCallback();
	}

};

} /* namespace DE */

#endif /* DE_FUNCTOR_H_ */
