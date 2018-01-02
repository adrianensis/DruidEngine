#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <ostream>
#include "Assert.h"

#include "BasicTypes.h"
#include "MathUtils.h"

namespace DE {

/*!
    \brief 3D Vector.
*/
class Vector3 {

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y, z;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Vector3();
	Vector3(f32 x, f32 y, f32 z);
	Vector3(const Vector3& other);
	~Vector3();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector3& set(const Vector3& rhs);
	Vector3& set(f32 x, f32 y, f32 z);
	Vector3& add(const Vector3& rhs);
	Vector3& sub(const Vector3& rhs);
	Vector3& mul(const Vector3& rhs);
	Vector3& div(const Vector3& rhs);
	Vector3& add(const f32 rhs);
	Vector3& sub(const f32 rhs);
	Vector3& mul(const f32 rhs);
	Vector3& div(const f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector3& v) const;
	Vector3& nor();
	f32 dst(const Vector3& v) const;
	f32 sqrdst(const Vector3& v) const;
	bool eq(const Vector3& v, f32 e) const; // epsilon equal
	bool eq(const Vector3& v) const; // equal
	Vector3& cross(const Vector3& v); // only defined for 3D space
	Vector3& lerp(const Vector3& target, f32 t);
	Vector3& nlerp(const Vector3& target, f32 t);
	Vector3& slerp(const Vector3& target, f32 t);
	f32 angle(const Vector3& v, const Vector3& n) const;
	f32 angle(const Vector3& v) const;
	Vector3& clamp(f32 maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Vector3& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Vector3& operator=(const Vector3& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector3& operator+=(const Vector3& rhs) {
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Vector3& operator-=(const Vector3& rhs) {
		return this->sub(rhs);
	}

	Vector3& operator*=(const Vector3& rhs) {
		return this->mul(rhs);
	}

	Vector3& operator/=(const Vector3& rhs) {
		return this->div(rhs);
	}

	Vector3& operator+=(const f32 rhs) {
		return this->add(rhs);
	}

	Vector3& operator-=(const f32 rhs) {
		return this->sub(rhs);
	}

	Vector3& operator*=(const f32 rhs) {
		return this->mul(rhs);
	}

	Vector3& operator/=(const f32 rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector3& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector3& rhs) {
		return !((*this) == rhs);
	}

	Vector3 operator+(const Vector3& rhs) const {
		return Vector3(*this) += rhs;
	}

	Vector3 operator-(const Vector3& rhs) const {
		return Vector3(*this) -= rhs;
	}

	Vector3 operator-() const {
		return Vector3(*this) *= -1;
	}

	Vector3 operator*(const Vector3& rhs) const {
		return Vector3(*this) *= rhs;
	}

	Vector3 operator/(const Vector3& rhs) const {
		return Vector3(*this) /= rhs;
	}

	Vector3 operator+(const f32 rhs) const {
		return Vector3(*this) += rhs;
	}

	Vector3 operator-(const f32 rhs) const {
		return Vector3(*this) -= rhs;
	}

	Vector3 operator*(const f32 rhs) const {
		return Vector3(*this) *= rhs;
	}

	Vector3 operator/(const f32 rhs) const {
		return Vector3(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t i) {
		ASSERT(i >= 0 && i < 3, "Index out of bounds.");

		return *(&x+i);
	}

	// read only
	f32 operator[](const size_t i) const {
		ASSERT(i >= 0 && i < 3, "Index out of bounds.");
		return *(&x+i);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector3& v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z  << ")";
		return out;
	}
};

} /* namespace DE */

#endif /* VECTOR3_H_ */
