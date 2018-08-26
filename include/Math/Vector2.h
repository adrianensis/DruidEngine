#ifndef DE_VECTOR2_H
#define DE_VECTOR2_H

#include <ostream>
#include "Assert.h"

#include "Basic.h"
#include "MathUtils.h"

namespace DE {

/*!
  \brief 2D Vector.
*/
class Vector2 : public DE_Class {

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	/*!
	   \brief Default Constructor.
	*/
	Vector2();
	Vector2(const f32 x, const f32 y);
	Vector2(const Vector2& other);
	~Vector2();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector2& set(const Vector2& rhs);
	Vector2& set(const f32 x, const f32 y);
	Vector2& add(const Vector2& rhs);
	Vector2& sub(const Vector2& rhs);
	Vector2& mul(const Vector2& rhs);
	Vector2& div(const Vector2& rhs);
	Vector2& add(const f32 rhs);
	Vector2& sub(const f32 rhs);
	Vector2& mul(const f32 rhs);
	Vector2& div(const f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector2& v) const;
	Vector2& nor();
	f32 dst(const Vector2& v) const;
	f32 sqrdst(const Vector2& v) const;
	bool eq(const Vector2& v, const f32 e) const; // epsilon equal
	bool eq(const Vector2& v) const; // equal
	Vector2& lerp(const Vector2& target, const f32 t);
	f32 angle(const Vector2& v) const;
	Vector2& clamp(const f32 maxLength);

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
		// can be parallelized with SIMD auto-vectorization
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

	Vector2& operator+=(const f32 rhs) {
		return this->add(rhs);
	}

	Vector2& operator-=(const f32 rhs) {
		return this->sub(rhs);
	}

	Vector2& operator*=(const f32 rhs) {
		return this->mul(rhs);
	}

	Vector2& operator/=(const f32 rhs) {
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

	Vector2 operator+(const f32 rhs) const {
		return Vector2(*this) += rhs;
	}

	Vector2 operator-(const f32 rhs) const {
		return Vector2(*this) -= rhs;
	}

	Vector2 operator-() const {
		return Vector2(*this) *= -1;
	}

	Vector2 operator*(const f32 rhs) const {
		return Vector2(*this) *= rhs;
	}

	Vector2 operator/(const f32 rhs) const {
		return Vector2(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t index) {
		DE_ASSERT(index >= 0 && index < 2, "Index out of bounds.");

		return *(&x+index);
	}

	// read only
	f32 operator[](const size_t index) const {
		DE_ASSERT(index >= 0 && index < 2, "Index out of bounds.");
		return *(&x+index);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector2& v) {
		out << "(" << v.x << ", " << v.y << ")";
		return out;
	}
};

} /* namespace DE */

#endif /* DE_VECTOR2_H */
