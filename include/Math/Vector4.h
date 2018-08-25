#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <ostream>
#include "Assert.h"

#include "Basic.h"
#include "MathUtils.h"

namespace DE {

/*!
  \brief 4D Vector.
*/
class Vector4 : public DE_Class /*16 bytes alignment*/ {

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y, z, w;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Vector4();
	Vector4(const f32 x, const f32 y, const f32 z, const f32 w);
	Vector4(const Vector4& other);
	~Vector4();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector4& set(const Vector4& rhs);
	Vector4& set(const f32 x, const f32 y, const f32 z, const f32 w);
	Vector4& add(const Vector4& rhs);
	Vector4& sub(const Vector4& rhs);
	Vector4& mul(const Vector4& rhs);
	Vector4& div(const Vector4& rhs);
	Vector4& add(const f32 rhs);
	Vector4& sub(const f32 rhs);
	Vector4& mul(const f32 rhs);
	Vector4& div(const f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector4& v) const;
	Vector4& nor();
	f32 dst(const Vector4& v) const;
	f32 sqrdst(const Vector4& v) const;
	bool eq(const Vector4& v, const f32 e) const; // epsilon equal
	bool eq(const Vector4& v) const; // equal
	Vector4& lerp(const Vector4& target, const f32 t);
	Vector4& clamp(const f32 maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Vector4& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Vector4& operator=(const Vector4& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector4& operator+=(const Vector4& rhs) {
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Vector4& operator-=(const Vector4& rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const Vector4& rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const Vector4& rhs) {
		return this->div(rhs);
	}

	Vector4& operator+=(const f32 rhs) {
		return this->add(rhs);
	}

	Vector4& operator-=(const f32 rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const f32 rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const f32 rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector4& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector4& rhs) const {
		return !((*this) == rhs);
	}

	Vector4 operator+(const Vector4& rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const Vector4& rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator*(const Vector4& rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const Vector4& rhs) const {
		return Vector4(*this) /= rhs;
	}

	Vector4 operator+(const f32 rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const f32 rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator-() const {
		return Vector4(*this) *= -1;
	}

	Vector4 operator*(const f32 rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const f32 rhs) const {
		return Vector4(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t index) {
		DE_ASSERT(index >= 0 && index < 4, "Index out of bounds.");

		return *(&x+index);
	}

	// read only
	f32 operator[](const size_t index) const {
		DE_ASSERT(index >= 0 && index < 4, "Index out of bounds.");
		return *(&x+index);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector4& v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return out;
	}
};

} /* namespace DE */

#endif /* VECTOR4_H_ */
