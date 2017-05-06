#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <ostream>
#include <cassert>
#include "Aligned16.h"
#include "MathUtils.h"

namespace DE {

class Vector2 : public Aligned16 {


public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	float x, y;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);
	~Vector2();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector2& set(const Vector2& rhs);
	Vector2& set(float x, float y);
	Vector2& add(const Vector2& rhs);
	Vector2& sub(const Vector2& rhs);
	Vector2& mul(const Vector2& rhs);
	Vector2& div(const Vector2& rhs);
	Vector2& add(const float rhs);
	Vector2& sub(const float rhs);
	Vector2& mul(const float rhs);
	Vector2& div(const float rhs);

	float len() const;
	float sqrlen() const;
	float max() const;
	float min() const;
	float dot(const Vector2& v) const;
	Vector2& nor();
	float dst(const Vector2& v) const;
	float sqrdst(const Vector2& v) const;
	bool eq(const Vector2& v, float e) const; // epsilon equal
	bool eq(const Vector2& v) const; // equal
	Vector2& lerp(const Vector2& target, float t);
	float angle(const Vector2& v) const;
	Vector2& clamp(float maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Vector2& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Vector2& operator=(const Vector2& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector2& operator+=(const Vector2& rhs) {
		// can be parallelized with sse auto-vectorization
		return this->add(rhs);
	}

	Vector2& operator-=(const Vector2& rhs) {
		return this->sub(rhs);
	}

	Vector2& operator*=(const Vector2& rhs) {
		return this->mul(rhs);
	}

	Vector2& operator/=(const Vector2& rhs) {
		return this->div(rhs);
	}

	Vector2& operator+=(const float rhs) {
		return this->add(rhs);
	}

	Vector2& operator-=(const float rhs) {
		return this->sub(rhs);
	}

	Vector2& operator*=(const float rhs) {
		return this->mul(rhs);
	}

	Vector2& operator/=(const float rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector2& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector2& rhs) const {
		return !((*this) == rhs);
	}

	Vector2 operator+(const Vector2& rhs) const {
		return Vector2(*this) += rhs;
	}

	Vector2 operator-(const Vector2& rhs) const {
		return Vector2(*this) -= rhs;
	}

	Vector2 operator*(const Vector2& rhs) const {
		return Vector2(*this) *= rhs;
	}

	Vector2 operator/(const Vector2& rhs) const {
		return Vector2(*this) /= rhs;
	}

	Vector2 operator+(const float rhs) const {
		return Vector2(*this) += rhs;
	}

	Vector2 operator-(const float rhs) const {
		return Vector2(*this) -= rhs;
	}

	Vector2 operator-() const {
		return Vector2(*this) *= -1;
	}

	Vector2 operator*(const float rhs) const {
		return Vector2(*this) *= rhs;
	}

	Vector2 operator/(const float rhs) const {
		return Vector2(*this) /= rhs;
	}

	// can be used for assignment
	float& operator[](const size_t i) {
		assert(i >= 0 && i < 2);

		return *(&x+i);
	}

	// read only
	float operator[](const size_t i) const {
		assert(i >= 0 && i < 2);
		return *(&x+i);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector2& v) {
		out << "(" << v.x << ", " << v.y << ")";
		return out;
	}
};

} /* namespace DE */

#endif /* VECTOR2_H_ */
