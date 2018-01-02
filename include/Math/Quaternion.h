#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <ostream>
#include "Assert.h"

#include "BasicTypes.h"
#include "MathUtils.h"
#include "Vector3.h"

namespace DE {

class Quaternion /*16 bytes alignment*/ {

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 w;
	Vector3 v;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Quaternion();
	Quaternion(f32 x, f32 y, f32 z, f32 w);
	Quaternion(const Vector3& v, f32 w);
	Quaternion(f32 theta_x, f32 theta_y, f32 theta_z);
	Quaternion(const Vector3& v);
	Quaternion(const Quaternion& other);
	~Quaternion();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Quaternion& set(f32 x, f32 y, f32 z, f32 w);
	Quaternion& set(const Vector3& v, f32 w);
	Quaternion& set(const Quaternion& rhs);
	Quaternion& add(const Quaternion& rhs);
	Quaternion& sub(const Quaternion& rhs);
	Quaternion& mul(const Quaternion& rhs);
	Quaternion& div(const Quaternion& rhs);
	Quaternion& add(const f32 rhs);
	Quaternion& sub(const f32 rhs);
	Quaternion& mul(const f32 rhs);
	Quaternion& div(const f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 dot(const Quaternion& q) const;
	Quaternion& nor();
	bool eq(const Quaternion& q, f32 e) const; // epsilon equal
	bool eq(const Quaternion& q) const; // equal
	Quaternion& conj();
	Quaternion& inv();
	Quaternion& lerp(const Quaternion& target, f32 t);
	Quaternion& nlerp(const Quaternion& target, f32 t);
	Quaternion& slerp(const Quaternion& target, f32 t);
	Quaternion& squad(); // TODO: implement
	Vector3 toEuler() const;
	f32 angle(const Quaternion& q) const;

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Quaternion& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Quaternion& operator=(const Quaternion& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Quaternion& operator+=(const Quaternion& rhs) {
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Quaternion& operator-=(const Quaternion& rhs) {
		return this->sub(rhs);
	}

	Quaternion& operator*=(const Quaternion& rhs) {
		return this->mul(rhs);
	}

	Quaternion& operator/=(const Quaternion& rhs) {
		return this->div(rhs);
	}

	Quaternion& operator+=(const f32 rhs) {
		return this->add(rhs);
	}

	Quaternion& operator-=(const f32 rhs) {
		return this->sub(rhs);
	}

	Quaternion& operator*=(const f32 rhs) {
		return this->mul(rhs);
	}

	Quaternion& operator/=(const f32 rhs) {
		return this->div(rhs);
	}

	bool operator==(const Quaternion& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Quaternion& rhs) const {
		return !((*this) == rhs);
	}

	Quaternion operator+(const Quaternion& rhs) const {
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(const Quaternion& rhs) const {
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(const Quaternion& rhs) const {
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(const Quaternion& rhs) const {
		return Quaternion(*this) /= rhs;
	}

	Quaternion operator+(const f32 rhs) const {
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(const f32 rhs) const {
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(const f32 rhs) const {
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(const f32 rhs) const {
		return Quaternion(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t i) {
		ASSERT(i >= 0 && i < 4, "Index out of bounds.");
		if(i == 3) return w;
		return v[i];
	}

	// read only
	f32 operator[](const size_t i) const {
		ASSERT(i >= 0 && i < 4, "Index out of bounds.");
		if(i == 3) return w;
		return v[i];
	}

	friend std::ostream& operator<<(std::ostream& out, const Quaternion& q) {
		out << "[(" << q.v.x << ", " << q.v.y << ", " << q.v.z << "), " << q.w << "]";
		return out;
	}
};

} /* namespace DE */

#endif /* QUATERNION_H_ */
